// Jacob Hartt, Derin Rawson, Lea Karsanbhai
// Serena Sullivan
// CS4220.002
// 04/17/2025

#pragma once

#include <openssl/err.h>
#include <openssl/ssl.h>

#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef int Socket;
typedef int Port;

#define DEFAULT_BUF_SIZE 64


enum ssl_mode { SSLMODE_SERVER, SSLMODE_CLIENT };
enum ssl_status { SSLSTATUS_OK, SSLSTATUS_WANT_IO, SSLSTATUS_FAIL };

struct ssl_client {
  int fd;

  SSL *ssl;

  BIO *rbio; // SSL reads from, we write to
  BIO *wbio; // SSL writes to, we read from

  // bytes to be written to socket
  char *write_buf;
  size_t write_len;

  // bytes waiting to be encrypted
  char *encrypt_buf;
  size_t encrypt_len;

  const char *last_state; // store previous state string

  // pointer to method to invoke when unencrypted bytes are available
  void (*io_on_read)(char *buf, size_t len);
};
extern struct ssl_client client;


// global ssl context (I don't like this)
extern SSL_CTX *ctx;


Socket create_socket(Port port);

void die(const char *msg);

void print_unencrypted_data(char *buf, size_t len);

void ssl_init(const char *certfile, const char *keyfile);

void ssl_client_init(struct ssl_client *p, int fd, enum ssl_mode mode);

int ssl_client_wants_write(struct ssl_client *client_p);

// read encrypted bytes from socket
int enc_sock_read();

// Write to encrypted socket
int enc_sock_write();

// read an encrypted charbuffer
int read_enc_cb(char *src, size_t len);

// run an ssl handshake
enum ssl_status do_ssl_handshake();

// prints ssl state
void print_ssl_state();

// gets ssl status
enum ssl_status get_ssl_status(SSL* ssl, int n);

// Add the new characters to the client's encrypt buffer, c-style
void queue_encrypted_bytes(const char *buf, size_t len);

// add data to encrypt_buf
void send_unencrypted_bytes(const char *buf, size_t len);

// read from stdin and queue for encyrption
void stdin_read();

// process queued data for encryption
int encrypt();
