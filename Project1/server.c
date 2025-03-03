#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 80

int main() {
    //define our buffers and vars
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(80);

    //open it
    int listensock = socket(AF_INET, SOCK_STREAM, 0);
    puts("created socket");

    bind(listensock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    puts("created bind");

    if(listen(listensock, 10) == -1) {
        puts("failed to listen");
        return -1;
    }

    //close it
    close(listensock);
    puts("closed socket");

}