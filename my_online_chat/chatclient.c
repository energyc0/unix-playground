#include "utils.h"
#include <arpa/inet.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

struct new_msg_create{
    int sockfd;
    pthread_barrier_t barrier;
};

static void enter_username(int sockfd, char* buf, int bufsize);
static void new_msg_thread(struct new_msg_create* create_info);

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
    
    char name[CLIENTNAME_LEN];
    enter_username(sockfd, name, sizeof(name));

    pthread_t thr;
    struct new_msg_create create_info;
    create_info.sockfd = sockfd;
    pthread_barrier_init(&create_info.barrier, NULL, 2);
    pthread_create(&thr, NULL, (void*)new_msg_thread, &create_info);
    pthread_barrier_wait(&create_info.barrier);
    pthread_barrier_destroy(&create_info.barrier);

    printf("You`ve entered the chat!\n");
    char buf[BUFSIZ];
    int len;
    while(fgets(buf, sizeof(buf), stdin) != NULL){
        /*
        //prompt a message
        printf("%s: ", name);
        fflush(stdin);
        */
        buf[strlen(buf) - 1] = '\0';
        dprintf(sockfd, "%s", buf);
    }

    return 0;
}

static void enter_username(int sockfd, char* buf, int bufsize){
    int len;
    while(1){
        printf("Enter your name: ");
        fflush(stdin);

        if(fgets(buf, bufsize, stdin) == NULL)
            exit(EXIT_SUCCESS);
        buf[strlen(buf)-1] = '\0';

        dprintf(sockfd, "%s", buf);

        if((len = read(sockfd, buf, bufsize)) <= 0)
            FATAL("read()");
        buf[len] = '\0';
        buf[strcspn(buf, "\r\n")] = '\0';
        if(strcmp(buf, REQ_SUCCESS) == 0)
            break;
        else
            printf("Your name is already in use!\n");
    }
}

static void new_msg_thread(struct new_msg_create* create_info){
    int sockfd = create_info->sockfd;
    pthread_barrier_wait(&create_info->barrier);

    char buf[BUFSIZ];
    int len;
    while((len = read(sockfd, buf, sizeof(buf))) > 0){
        if(write(STDOUT_FILENO, buf, len) <= 0)
            FATAL("write()");
    }
    if(len < 0)
        printf("Server closed connection...\n");

}