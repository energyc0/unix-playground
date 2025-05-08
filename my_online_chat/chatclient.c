#include "utils.h"
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>

static void enter_username(int sockfd, char* buf, int bufsize);

int main(int argc, char** argv){
    if(argc != 2)
        FATAL("Server IP-address expected");

    int sockfd;
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        FATAL("socket(): %s", strerror(errno));

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(CHATSERVER_PORT);
    if(inet_pton(AF_INET, argv[1], &saddr.sin_addr) < 0)
        FATAL("inet_pton(): %s", strerror(errno));

    if(connect(sockfd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0)
        FATAL("connect() %s", strerror(errno));
    
    char buf[BUFSIZ];
    int len;
    while((len = read(sockfd, buf, sizeof(buf))) > 0){
        buf[len-1] = '\0';
        printf("%s\n", buf);

        if((len = read(STDIN_FILENO, buf, sizeof(buf))) <= 0)
            break;
        if(write(sockfd, buf, len) <0)
            FATAL("write(): %s", strerror(errno));
    }

    return 0;
}

static void enter_username(int sockfd, char* buf, int bufsize){
    int len;
    while((len = read(sockfd, buf, bufsize)) > 0){
        buf[len-1] = '\0';
        printf("%s",buf);
        fflush(stdin);

        if((len = read(STDIN_FILENO, buf, bufsize)) <= 0)
            exit(EXIT_SUCCESS);
        if(write(sockfd, buf, len) < 0)
            FATAL("write(): %s", strerror(errno));

        if((len = read(STDIN_FILENO, buf, bufsize)) <= 0)
            exit(EXIT_SUCCESS);
        if(strcmp(REQ_SUCCESS, buf) == 0) //entered name successfully
            break;

        printf("Your name is already in use!\n");
    }
}