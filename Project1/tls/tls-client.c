// Jacob Hartt, Derin Rawson, Lea Karsanbhai
// Serena Sullivan
// CS4220.002
// 04/17/2025

#include <stdio.h>


#include <arpa/inet.h>
// #include <openssl/bio.h>
#include <openssl/ssl.h>
// #include <openssl/err.h>


#include "tls-server.h"
#include "tls-utils.h"


int main() {
	// // Initialize SSL with no options, no settings
	// if (!OPENSSL_init_ssl(0, NULL)) {
	// 	fprintf(stderr, "Failed to initialize OpenSSL\n");
	// 	return 1;
	// } else {
	// 	printf("Successfully initialized OpenSSL\n");
	// }

	int port = 443;

	// placeholder ip, might not be necessary
	char* host_ip = "2600:9000:225d:600:14:c251:2440:93a1";
	const char *host_name = "api.huobi.pro";

	// set up IPv6 streaming socket
	int ip_family = AF_INET6;
	int sock = socket(ip_family, SOCK_STREAM, 0);
	if (sock < 0)
		die("failed to set up socket");
	else
		printf("successfully set up socket\n");

	if (ip_family == AF_INET6) {
		// init sock addr
		struct sockaddr_in6 addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin6_family = ip_family;
		addr.sin6_port = htons(port);

		// convert text ip to binary ip address
		if (inet_pton(ip_family, host_ip, &(addr.sin6_addr)) <= 0)
			die("failed to convert text ip to binary");
		else
			printf("converted ip from text to binary\n");

		// attempt to connect
		if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
			die("unable to initiate connection on socket");
		else 
			printf("initiated connection on socket\n");
	}

	if (ip_family == AF_INET) {
		// init sock addr
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = ip_family;
		addr.sin_port = htons(port);

		// convert text ip to binary ip address
		if (inet_pton(ip_family, host_ip, &(addr.sin_addr)) <= 0)
			die("failed to convert text ip to binary");
		else
			printf("converted ip from text to binary\n");

		// attempt to connect
		if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
			die("unable to initiate connection on socket");
		else 
			printf("initiated connection on socket\n");
	}

	printf("TLS (Not)\n");

	return 0;
}
