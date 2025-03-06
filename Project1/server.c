#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)




/*
 *Do we need to support IPv4 and IPv6?
// Support both IPv4 and IPv6.
int option = 0;
if (setsockopt(listening_socket, IPPROTO_IPV6, IPV6_V6ONLY, (void*)&option, sizeof(option)))
{
    fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRNO());
    return 1;
}
*/

void handleConnection(int listening_socket){
    puts("connection established");
}

int main(void)
{
    // Find local address that web server should bind to.
    printf("Configuring local addresses...\n");
    struct addrinfo loc_address;
    memset(&loc_address, 0, sizeof(loc_address));
    loc_address.ai_family = AF_INET;            // Looking for IPv4 address
    loc_address.ai_socktype = SOCK_STREAM;      // TCP
    loc_address.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &loc_address, &bind_address);


    /* Create the listening socket that will accept new connections. */
    printf("Creating socket...\n");
    SOCKET listening_socket = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);

    // Make sure that the call to socket() was successful.
    if (!ISVALIDSOCKET(listening_socket))
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    // Use bind() to associate the socket with the address from getaddrinfo()
    printf("Binding socket to local address...\n");
    if (bind(listening_socket, bind_address->ai_addr, bind_address->ai_addrlen))
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    freeaddrinfo(bind_address); // Release the address memory

    //Start to listen for connections on socket that has been bound to the local address.
    printf("Listening...\n");
    if (listen(listening_socket, 5) < 0)
    {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    // Accept any incoming connection.
    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(listening_socket, (struct sockaddr *)&client_address, &client_len);

    if (!ISVALIDSOCKET(socket_client))
    {
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    } else {
        //code to process the connection
        handleConnection(listening_socket);
    }


    //Close the client connection
    printf("Closing connection...\n");
    CLOSESOCKET(socket_client);

    printf("Closing listening socket...\n");
    CLOSESOCKET(listening_socket);

    printf("Finished.\n");
    return 0;
}
