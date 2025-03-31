// Jacob Hartt, Derin Rawson, Lea Karsanbhai
// Serena Sullivan
// CS4220.002
// 04/17/2025

#include <stdio.h>


// #include <openssl/bio.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
// #include <openssl/err.h>


#include "tls-server.h"
#include "tls-utils.h"

#define PORT 55555


int main() {
	// // Initialize SSL with no options, no settings
	// if (!OPENSSL_init_ssl(0, NULL)) {
    //     fprintf(stderr, "Failed to initialize OpenSSL\n");
    //     return 1;
    // } else {
	// 	printf("Successfully initialized OpenSSL\n");
	// }

	char ip_str[INET_ADDRSTRLEN];
	int port = PORT;

	// Setup IPv4 streaming socket with default protocol
	int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sock < 0)
    	die("socket failed to initialize");
	else
		printf("socket initialized\n");

	// Force socket to allow reuse of local addresses at the socket level, but not network level
	int true_var = 1;
	if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &true_var, sizeof(true_var)) < 0)
		die("failed to set socket options");
	else 
		printf("socket configured with SO_REUSEADDR\n");
	
	// Configure server address port
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr)); // Set everything in the structure to 0
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // host to network long
	serv_addr.sin_port = htons(port); // host to network short

	if (bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		die("failed to bind socket");
	else
		printf("successfully bound socket to address\n");

	// Start listening to server socket with a backlog of 128
	if (listen(serv_sock, 128) < 0)
		die("failed to begin listening to socket");
	else
		printf("started listening to socket\n");

	int client_sock;
	struct sockaddr_in peer_addr;
	socklen_t peer_addr_len = sizeof(peer_addr);

	struct pollfd fd_set[2];
	memset(&fd_set, 0, sizeof(fd_set)); // init fd_set to 0

	fd_set[0].fd = STDIN_FILENO; // Set file descriptor to stdin
	fd_set[0].events = POLLIN; // configure to read data

	// if (ssl_init("server.crt", "server.key") < 0)
	// 	die("failed to init ssl");
	// else
	// 	printf("initialized ssl");
	ssl_init("server.crt", "server.key");

	printf("TLS (Not)\n");

	return 0;
}
