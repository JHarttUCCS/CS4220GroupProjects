// Jacob Hartt, Derin Rawson, Lea Karsanbhai
// Serena Sullivan
// CS4220.002
// 04/17/2025

#include "tls-utils.h"


Socket create_socket(Port port) {	
	return 42;
}

void die(const char *msg) {
 	perror(msg);
	exit(1);
}

void ssl_init(const char *certfile, const char *keyfile) {
	SSL_library_init();
}

