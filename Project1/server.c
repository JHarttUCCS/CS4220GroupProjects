#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/params.h>
#include <openssl/ssl.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define GETSOCKETERRNO() (errno)
#define PORT "8080"
#define BUFSIZE 1024

typedef int SOCKET;

SOCKET create_socket(const char* host, const char *port);
int handle_connection (SSL *client_ssl);

int main(void)
{

    //initialize OpenSSL
    SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
    SSL_CTX *sslctx = SSL_CTX_new(TLS_method());
    if(sslctx == NULL) {
        fprintf(stderr, "%s", "Failed to intialize OpenSSL. SSL context returned NULL\n");
        return -1;
    } else {
        puts("Loaded OpenSSL");
    }

    // Specify an HMAC cipher suite https://ciphersuite.info/cs/TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384/
    SSL_CTX_set_cipher_list(sslctx, "ECDHE-RSA-AES256-SHA384");

    if(SSL_CTX_set_min_proto_version(sslctx, TLS1_2_VERSION) != 1) {
        fprintf(stderr, "%s", "Failed to set min TLS to 1.2\n");
        return -1;
    }
    
    if(SSL_CTX_use_PrivateKey_file(sslctx, "keys/server.key", SSL_FILETYPE_PEM) != 1) {
        fprintf(stderr, "%s", "Failed to load private key\n");
        return -1;
    } else {
        puts("loaded private key");
    }

    if (SSL_CTX_use_certificate_file(sslctx, "keys/server.crt", SSL_FILETYPE_PEM) != 1) {
        fprintf(stderr, "%s", "Failed to load public key\n");
        return -1;
    } else {
        puts("Loaded public key");
    }

    //fireup SSL
    SSL *sslclient = SSL_new(sslctx);


    //establish socket connection
    SOCKET listen_socket = create_socket(0, PORT);

    /* Accept any incoming connection. */
    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(listen_socket, (struct sockaddr *)&client_address, &client_len);
    //config SSL to use socket
    SSL_set_accept_state(sslclient);

    if(SSL_set_fd(sslclient, socket_client) != 1) {
        fprintf(stderr, "%s\n", "SSL failed to bind to socket fd");
        return -1;
    }

    if (!ISVALIDSOCKET(socket_client))
    {
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
        return -1;
    }

    //do client handshake
    if (SSL_do_handshake(sslclient) != 1) {
        fprintf(stderr, "%s", "SSL handshake failed\n");
        return -1;
    }

    /* Read client request. */
    printf("Reading request...\n");
    char *request = (char*) malloc(BUFSIZE);
    //int bytes_received = recv(socket_client, request, BUFSIZE, 0);
    size_t bytesRead = 0;
    if (SSL_read_ex(sslclient, request, BUFSIZE, &bytesRead) != 1) {
        fprintf(stderr, "%s", "SSL_read Failed\n");
        return -1;
    }

    request[bytesRead] = 0;
    char *q = strstr(request, "\r\n\r\n");
    if (q)
    {
        // Ensure that it is a GET request.
        if (strncmp("GET /", request, 5))
        {
            fprintf(stderr, "Bad request. (%s)\n", request);
            return -1;
        }

        // Send message back to client and handle errors with program crash
        if(handle_connection(sslclient) != 1) {
            return -1;
        } 
    }

    //shutdown SSL
    puts("Closing SSL");
    SSL_shutdown(sslclient);

    //Close the client connection
    printf("Closing connection...\n");
    CLOSESOCKET(socket_client);

    printf("\nClosing socket...\n");
    CLOSESOCKET(listen_socket);

    puts("Freeing memory");
    //free mem
    free(request);

    printf("Finished.\n");
    return 0;
}

SOCKET create_socket(const char* host, const char *port)
{
    /* Find local address that server should bind to. */
    printf("Configuring local addresses...\n");
    struct addrinfo loc_address;
    memset(&loc_address, 0, sizeof(loc_address));
    loc_address.ai_family = AF_INET;            // Looking for IPv4 address
    loc_address.ai_socktype = SOCK_STREAM;      // TCP
    loc_address.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    if (getaddrinfo(0, PORT, &loc_address, &bind_address))
    {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        return -1;
    }

    /* Create the listening socket that will accept new connections. */
    printf("Creating socket...\n");
    SOCKET listening_socket = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);

    // Make sure that the call to socket() was successful.
    if (!ISVALIDSOCKET(listening_socket))
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return -1;
    }

    /* Use bind() to associate the socket with the address from getaddrinfo(). */
    printf("Binding socket to local address...\n");
    if (bind(listening_socket, bind_address->ai_addr, bind_address->ai_addrlen))
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return -1;
    }

    freeaddrinfo(bind_address); // Release the address memory

    /* Start to listen for connections on socket that has been bound to the local address. */
    printf("Listening...\n");
    if (listen(listening_socket, 5) < 0)
    {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return -1;
    }

    return listening_socket;
}

/* Receive and send data on the connection. */
int handle_connection (SSL *client_ssl)
{
    /* Respond to client from server. */
    printf("Sending response...\n");
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Connection: close\r\n"
        "Server: HTTPS Server\r\n"
        "Content-Length: 29\r\n\r\n"
        "Hello from the HTTPS server!\n";

    int bytes_sent = SSL_write(client_ssl, response, strlen(response));
    if (bytes_sent < 0)
    {
        perror("Error sending data to client");
        return -1;
    }
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));
    return 1;
}
