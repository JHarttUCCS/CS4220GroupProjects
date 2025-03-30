#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    //define our buffers and vars
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(8000);

    //open it
    int listensock = socket(AF_INET, SOCK_STREAM, 0);
    puts("created socket");

    if (connect(listensock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == 0){
        puts("connected");
    } else {
        puts("failed");
        return -1;
    }
    
    //500 byte buffer
    char *tcpbuff = (char*) malloc(10000);

    //GET seg string 
    strcpy(tcpbuff, "GET  / HTTP/1.1\r\n Host: 127.0.0.1:8000\r\nUser-Agent: SimpleHttpClient/1.0\r\nAccept: text/html\r\nAccept-Language:en-US\r\nAccept-Encoding: identity\r\n\r\n");
    
    //send it and zero
    write(listensock, tcpbuff, 10000);
    bzero(tcpbuff, 500);

    //read reply
    read(listensock, tcpbuff, 10000);

    //print reply
    printf("\n%s\n", tcpbuff);

    //free mem
    free(tcpbuff);

    //close it
    close(listensock);
    puts("Good bye");
}

