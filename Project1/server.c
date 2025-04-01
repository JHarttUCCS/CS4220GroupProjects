#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/ssl.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define GETSOCKETERRNO() (errno)
#define PORT "8080"
#define BUFSIZE 1024


void handleConnection(int listening_socket){
    puts("connection established");
}


int main(void)
{
    typedef int SOCKET;

    /* Find local address that web server should bind to. */
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
    }


    /* Create the listening socket that will accept new connections. */
    printf("Creating socket...\n");
    SOCKET listening_socket = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);

    // Make sure that the call to socket() was successful.
    if (!ISVALIDSOCKET(listening_socket))
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    /* Use bind() to associate the socket with the address from getaddrinfo(). */
    printf("Binding socket to local address...\n");
    if (bind(listening_socket, bind_address->ai_addr, bind_address->ai_addrlen))
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    freeaddrinfo(bind_address); // Release the address memory

    /* Start to listen for connections on socket that has been bound to the local address. */
    printf("Listening...\n");
    if (listen(listening_socket, 5) < 0)
    {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    /* Accept any incoming connection. */
    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(listening_socket, (struct sockaddr *)&client_address, &client_len);

    if (!ISVALIDSOCKET(socket_client))
    {
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    /* Read a message from the client. */
    char buffer[BUFSIZE];
    read(socket_client, buffer, BUFSIZE);
    printf("Received message: %s\n", buffer);

    /* Send a message to the client. */
    char *reply = "Message from server: Hello \n";
    send(socket_client, reply, strlen(reply), 0);


    /* Close the client connection */
    printf("Closing connection...\n");
    CLOSESOCKET(socket_client);

    printf("Closing listening socket...\n");
    CLOSESOCKET(listening_socket);

    printf("Finished.\n");
    return 0;
}
