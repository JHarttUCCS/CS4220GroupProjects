// Jacob Hartt, Derin Rawson, Lea Karsanbhai
// Serena Sullivan
// CS4220.002
// 04/17/2025

#include <openssl/ssl.h>

#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef int Socket;
typedef int Port;


Socket create_socket(Port port);

void die(const char *msg);

void ssl_init(const char *certfile, const char *keyfile);
