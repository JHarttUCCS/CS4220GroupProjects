#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define GETSOCKETERRNO() (errno)
#define PORT "8080"
#define BUFSIZE 1024

typedef int SOCKET;

SOCKET create_socket(const char* host, const char *port);
void handle_connection (SOCKET socket_client);

int main(void)
{
    SOCKET listen_socket = create_socket(0, PORT);

    /* Accept any incoming connection. */
    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(listen_socket, (struct sockaddr *)&client_address, &client_len);

    if (!ISVALIDSOCKET(socket_client))
    {
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    /* Read client request. */
    printf("Reading request...\n");
    char request[BUFSIZE];
    int bytes_received = recv(socket_client, request, BUFSIZE, 0);

    // Check that recv() > 0
    if (bytes_received <= 0)
    {
        fprintf(stderr, "recv() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }

    // Detect if an HTTP header has been received.
    else
    {
        request[bytes_received] = 0;
        char *q = strstr(request, "\r\n\r\n");
        if (q)
        {
            // Ensure that it is a GET request.
            if (strncmp("GET /", request, 5))
            {
                fprintf(stderr, "Bad request. (%s)\n", request);
                exit(1);
            }

            // Rend message back to client.
            handle_connection(socket_client);
        }
    }


    //Close the client connection
    printf("Closing connection...\n");
    CLOSESOCKET(socket_client);

    printf("\nClosing socket...\n");
    CLOSESOCKET(listen_socket);

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
        exit(1);
    }

    /* Create the listening socket that will accept new connections. */
    printf("Creating socket...\n");
    SOCKET listening_socket = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);

    // Make sure that the call to socket() was successful.
    if (!ISVALIDSOCKET(listening_socket))
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }

    /* Use bind() to associate the socket with the address from getaddrinfo(). */
    printf("Binding socket to local address...\n");
    if (bind(listening_socket, bind_address->ai_addr, bind_address->ai_addrlen))
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }

    freeaddrinfo(bind_address); // Release the address memory

    /* Start to listen for connections on socket that has been bound to the local address. */
    printf("Listening...\n");
    if (listen(listening_socket, 5) < 0)
    {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }

    return listening_socket;
}

/* Receive and send data on the connection. */
void handle_connection (SOCKET client_socket)
{
    /* Respond to client from server. */
    printf("Sending response...\n");
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Connection: close\r\n"
        "Server: HTTP Server\r\n\r\n"
        "Hello from the HTTP server!\n";

    int bytes_sent = send(client_socket, response, strlen(response), 0);
    if (bytes_sent < 0)
    {
        perror("Error sending data to client");
        exit(1);
    }
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));
}
