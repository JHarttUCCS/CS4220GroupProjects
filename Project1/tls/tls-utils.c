// Jacob Hartt, Derin Rawson, Lea Karsanbhai
// Serena Sullivan
// CS4220.002
// 04/17/2025

#include "tls-utils.h"


SSL_CTX *ctx = NULL;  // Define it only here
struct ssl_client client; // Define it here


Socket create_socket(Port port) {	
	return 42;
}

void die(const char *msg) {
 	perror(msg);
	exit(1);
}

void print_unencrypted_data(char *buf, size_t len) {
  printf("%.*s", (int)len, buf);
}

void ssl_init(const char *certfile, const char *keyfile) {
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
#if OPENSSL_VERSION_MAJOR < 3
	ERR_load_BIO_strings(); // deperecated after v3.0
	ERR_load_CRYPTO_strings();
#endif

	// create ssl server context
	ctx = SSL_CTX_new(TLS_method());
	if (!ctx)
		die("failed to create ssl context");
	else
		printf("created ssl context\n");

	if (certfile && keyfile) {
		if (SSL_CTX_use_certificate_file(ctx, certfile, SSL_FILETYPE_PEM) != 1)
			die("failed to load certificate");
		else
			printf("loaded certificate\n");

		if (SSL_CTX_use_PrivateKey_file(ctx, keyfile, SSL_FILETYPE_PEM) != 1)
			die("failed to load key");
		else
			printf("loaded key\n");

		if (SSL_CTX_check_private_key(ctx) != 1)
			die("key and certificate don't match");
		else
			printf("key and certificate match\n");
	}

	// disable ssl2 and ssl3
	SSL_CTX_set_options(ctx, SSL_OP_ALL|SSL_OP_NO_SSLv2|SSL_OP_NO_SSLv3);
}


void ssl_client_init(struct ssl_client *p, int fd, enum ssl_mode mode) {
	memset(p, 0, sizeof(struct ssl_client));

	p->fd = fd;

	p->rbio = BIO_new(BIO_s_mem());
	p->wbio = BIO_new(BIO_s_mem());
	p->ssl = SSL_new(ctx);

	if (mode == SSLMODE_SERVER)
		SSL_set_accept_state(p->ssl);
	else if (mode == SSLMODE_CLIENT)
		SSL_set_connect_state(p->ssl);

	SSL_set_bio(p->ssl, p->rbio, p->wbio);

	p->io_on_read = print_unencrypted_data;
}


int ssl_client_wants_write(struct ssl_client *client_p) {
  return (client_p->write_len > 0);
}


int enc_sock_read() {
	// Read data and get size of it
	char buf[DEFAULT_BUF_SIZE];
	ssize_t n = read(client.fd, buf, sizeof(buf));

	// read encrypted data if buffer is greater than zero
	if (n > 0)
		return read_enc_cb(buf, (size_t)n);
	else
		return -1;
}

int enc_sock_write() {
	// write data to the socket
	ssize_t n = write(client.fd, client.write_buf, client.write_len);

	//  move the written remove the data from the buffer
	if (n > 0) {
		// if the number of chars to write is less than the lients write lenghts, move them into the write buffer from outside
		if ((size_t) n < client.write_len)
			memmove(client.write_buf, client.write_buf+n, client.write_len-n);
		
		client.write_len -= n;
		client.write_buf = (char *) realloc(client.write_buf, client.write_len);
		
		return 0;
	} else {
		return -1;
	}
}


int read_enc_cb(char *src, size_t len) {
	char buf[DEFAULT_BUF_SIZE];
	enum ssl_status status;
	int n;

	while (len > 0) {
		n = BIO_write(client.rbio, src, len);

		// bio fails, return error
		if (n <= 0)
			return -1;

		// iterate forward on the charbuffer
		src += n;
		len -= n;

		// if ssl is not already initialized, attempt to initialize
		if (!SSL_is_init_finished(client.ssl)) {
			if (do_ssl_handshake() == SSLSTATUS_FAIL)
				return -1;
			if (!SSL_is_init_finished(client.ssl))
				return 0;
		}

		// now that data is in the buffer, we can read the data

		// read all bytes into buf 64 bytes at a time
		do {
			n = SSL_read(client.ssl, buf, sizeof(buf));
			if (n > 0)
				client.io_on_read(buf, (size_t)n);
		} while (n > 0);

		status = get_ssl_status(client.ssl, n);

		// the ssl request may want to write bytes as well. happens rarely when the peer requests SSL renegotiation.
		if (status == SSLSTATUS_WANT_IO) {
			do {
				n = BIO_read(client.wbio, buf, sizeof(buf));
				if (n > 0)
					queue_encrypted_bytes(buf, n);
				else if (!BIO_should_retry(client.wbio))
					return -1;
			} while (n > 0);
		}

		if (status == SSLSTATUS_FAIL)
			return -1;
	}

	return 0;
}


enum ssl_status do_ssl_handshake() {
	char buf[DEFAULT_BUF_SIZE];
	enum ssl_status status;

	// do the actual handshake
	print_ssl_state();
	int n = SSL_do_handshake(client.ssl);
	print_ssl_state();
	status = get_ssl_status(client.ssl, n);

	// if ssl requested write, queue up bytes until none left
	if (status == SSLSTATUS_WANT_IO) {
		do {
			n = BIO_read(client.wbio, buf, sizeof(buf));
			if (n > 0)
				queue_encrypted_bytes(buf, n);
			else if (!BIO_should_retry(client.wbio))
				return SSLSTATUS_FAIL;
		} while (n > 0);
	}
	
	return status;
}


void print_ssl_state() {
	const char *current_state = SSL_state_string_long(client.ssl);
	if (current_state != client.last_state) {
		if (current_state)
			printf("SSL STATE: %s\n", current_state);
		client.last_state = current_state;
	}
}


enum ssl_status get_ssl_status(SSL* ssl, int n)
{
  switch (SSL_get_error(ssl, n))
  {
   case SSL_ERROR_NONE:
    return SSLSTATUS_OK;
   case SSL_ERROR_WANT_WRITE:
   case SSL_ERROR_WANT_READ:
    return SSLSTATUS_WANT_IO;
   case SSL_ERROR_ZERO_RETURN:
   case SSL_ERROR_SYSCALL:
   default:
    return SSLSTATUS_FAIL;
  }
}


void queue_encrypted_bytes(const char *buf, size_t len) {
	client.write_buf = (char *) realloc(client.write_buf, client.write_len + len);
	memcpy(client.write_buf + client.write_len, buf, len);
	client.write_len += len;
}

void send_unencrypted_bytes(const char *buf, size_t len) {
  client.encrypt_buf = (char *) realloc(client.encrypt_buf, client.encrypt_len + len);
  memcpy(client.encrypt_buf + client.encrypt_len, buf, len);
  client.encrypt_len += len;
}


void stdin_read() {
	char buf[DEFAULT_BUF_SIZE];

	// Read bytes and send them
	ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
	if (n > 0)
		send_unencrypted_bytes(buf, (size_t) n);
}

int encrypt() {
	char buf[DEFAULT_BUF_SIZE];
	enum ssl_status status;

	if (!SSL_is_init_finished(client.ssl))
		return 0;

	// while there's stuff to encrypt
	while (client.encrypt_len > 0) {
		// perform actual encryption
		int n = SSL_write(client.ssl, client.encrypt_buf, client.encrypt_len);
		status = get_ssl_status(client.ssl, n);

		if (n > 0) {
			// consume waiting bytes
			if ((size_t) n < client.encrypt_len)
				memmove(client.encrypt_buf, client.encrypt_buf+n, client.encrypt_len-n);
			client.encrypt_len -= n;
			client.encrypt_buf = (char*)realloc(client.encrypt_buf, client.encrypt_len);
		
			// queue the encrypted data for socket write
			do {
				n = BIO_read(client.wbio, buf, sizeof(buf));
				if (n > 0)
					queue_encrypted_bytes(buf, n);
				else if (!BIO_should_retry(client.wbio))
					return -1;
			} while (n > 0);
		}
		
		if (status == SSLSTATUS_FAIL)
			return -1;
		
		if (n == 0)
			break;
	}

	return 0;
}
