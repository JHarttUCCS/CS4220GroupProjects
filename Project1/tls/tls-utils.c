// Jacob Hartt, Derin Rawson, Lea Karsanbhai
// Serena Sullivan
// CS4220.002
// 04/17/2025

#include "tls-utils.h"


SSL_CTX *ctx = NULL;  // Define it only here


Socket create_socket(Port port) {	
	return 42;
}

void die(const char *msg) {
 	perror(msg);
	exit(1);
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
	return;
}
