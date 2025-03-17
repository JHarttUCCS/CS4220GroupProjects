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
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(PORT);

    //open it
    int listensock = socket(AF_INET, SOCK_STREAM, 0);
    puts("created socket");

    if (connect(listensock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == 0){
        puts("connected");
    } else {
        puts("failed")
        return -1
    }
    
    //send GET

    

    //close it
    close(listensock);
    puts("closed socket");
}

