#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <arpa/inet.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>

#define PORT 8080
#define BUFSIZE 1024

int main() {

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

    if(SSL_CTX_set_min_proto_version(sslctx, TLS1_2_VERSION) != 1) {
        fprintf(stderr, "%s", "Failed to set min TLS to 1.2\n");
        return -1;
    }

    //fireup SSL
    SSL *sslclient = SSL_new(sslctx);

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
        puts("failed");
        return -1;
    }

    //config SSL to use socket
    SSL_set_connect_state(sslclient);

    if(SSL_set_fd(sslclient, listensock) != 1) {
        fprintf(stderr, "%s\n", "SSL failed to bind to socket fd");
        return -1;
    }
    
    //byte buffer
    char *tcpbuff = (char*) malloc(BUFSIZE);

    //GET seg string 
    strcpy(tcpbuff, "GET / HTTP/1.1\r\n"
        "Host: 127.0.0.1:8000\r\n"
        "User-Agent: SimpleHttpClient/1.0\r\n"
        "Accept: text/html\r\n"
        "Accept-Language:en-US\r\n"
        "Accept-Encoding: identity\r\n\r\n");
    
    //send it and zero
    SSL_write(sslclient, tcpbuff, BUFSIZE);
    bzero(tcpbuff, BUFSIZE);

    //read reply
    SSL_read(sslclient, tcpbuff, BUFSIZE);

    //remove headers
    char *q = strstr(tcpbuff, "\r\n\r\n")+4;
    if(q) {
        //print reply
        printf("%s", q);
    }

    //free mem
    free(tcpbuff);

    //close it
    SSL_shutdown(sslclient);
    close(listensock);
    puts("Good bye");
}

