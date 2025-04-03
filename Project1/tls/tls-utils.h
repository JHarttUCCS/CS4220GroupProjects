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


enum ssl_mode { SSLMODE_SERVER, SSLMODE_CLIENT };

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

  // method to invoke when unencrypted bytes are available
  void (*io_on_read)(char *buf, size_t len);
} client;


// global ssl context (I don't like this)
extern SSL_CTX *ctx;


Socket create_socket(Port port);

void die(const char *msg);

void ssl_init(const char *certfile, const char *keyfile);

void ssl_client_init(struct ssl_client *p, int fd, enum ssl_mode mode);

