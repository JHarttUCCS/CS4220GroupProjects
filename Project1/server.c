#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 80

int main() {
    //open it
    int listensock = socket(AF_INET, SOCK_STREAM, 0);
    puts("created socket");

    //close it
    close(listensock);
    puts("closed socket");

}