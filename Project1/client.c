// Jacob Hartt, Derin Rawson, Lea Karsanbhai
// Serena Sullivan
// CS4220.002
// 04/17/2025

#define PORT 80 // http port
#define MAX_PENDING_CONNECTIONS 10


#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


int main() {
    //define our buffers and vars
    struct sockaddr_in clientaddr; // sockaddr_in is an internet address
    clientaddr.sin_family = AF_INET; // AF_INET means this is basically an IPv4 socket with a port #
    clientaddr.sin_addr.s_addr = htonl(INADDR_ANY); // htonl() converts a host integer to a
													// network long (sometimes differnt byte order)
    clientaddr.sin_port = htons(PORT); // htons() converts a host integer to a network 
									   // short (sometimes differnt byte order)

    //open it
    int listensock = socket(AF_INET, SOCK_STREAM, 0); // open an IPv4 socket for a stream connection 
													  // with the default protocol 
    puts("created socket");

    bind(listensock, (struct sockaddr*)&clientaddr, sizeof(clientaddr)); // bind the socket to the clientaddress
    puts("created bind");

    if(listen(listensock, MAX_PENDING_CONNECTIONS) == -1) { // attempt to start listening to the socket,
															// with a maximum of 10 pending connections
        puts("failed to start listening");
        return -1;
    }

    //close it
    close(listensock);
    puts("closed socket");
}

