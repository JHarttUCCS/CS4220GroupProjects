// Jacob Hartt, Derin Rawson, Lea Karsanbhai
// Serena Sullivan
// CS4220.002
// 04/17/2025

#include <stdio.h>

// #include <openssl/bio.h>
#include <openssl/ssl.h>
// #include <openssl/err.h>


#include "tls-utils.h"


int main() {
	// Initialize SSL with no options, no settings
	if (!OPENSSL_init_ssl(0, NULL)) {
        fprintf(stderr, "Failed to initialize OpenSSL\n");
        return 1;
    } else {
		printf("Successfully initialized OpenSSL\n");
	}

	printf("TLS (Not)\n");

	return 0;
}
