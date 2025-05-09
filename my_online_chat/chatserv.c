#include "clientlist.h"
#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

struct service_create{
    int clientfd;
    pthread_barrier_t barrier;
};

static void client_service(struct service_create* create_info);
//static int receive_message(int clientfd, char* buf, int buflen);
static int get_user_name(int clientfd, char* name, int namesz);

//static int is_name_busy(char* name); //check whether the name is in use and add it to the list if it is not

int main(){
    int sockfd;
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        FATAL("socket(): %s", strerror(errno));

    int opt = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
        FATAL("setsockopt(): %s", strerror(errno));

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(CHATSERVER_PORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0)
        FATAL("bind(): %s", strerror(errno));

    if(listen(sockfd, CHATSERVER_LISTENERS) < 0)
        FATAL("listen(): %s", strerror(errno));

    printf("Server started at %s:%d\n", inet_ntoa(saddr.sin_addr), CHATSERVER_PORT);
    while(1){
        int clientfd = accept(sockfd, 0, 0);
        if(clientfd < 0){
            fprintf(stderr, "failed to accept a client: %s\n", strerror(errno));
            continue;
        }

        pthread_t thr;
        struct service_create create_info;
        create_info.clientfd = clientfd;
        pthread_barrier_init(&create_info.barrier, NULL, 2);
        pthread_create(&thr, NULL, (void*)client_service, &create_info);
        pthread_barrier_wait(&create_info.barrier);
        pthread_barrier_destroy(&create_info.barrier);
    }
}

static void client_service(struct service_create* create_info){
    int clientfd = create_info->clientfd;
    pthread_barrier_wait(&create_info->barrier);

    char name[CLIENTNAME_LEN];
    int namelen;
    if((namelen = get_user_name(clientfd, name, sizeof(name))) <= 0){
        fprintf(stderr, "failed to get a user name\n");
        return;
    }

    char buf[BUFSIZ];
    int len;    
    clientlist_printf_msg("\x1b[1;32m%s\x1b[0m entered the chat\n",name);

    while((len = read(clientfd, buf, sizeof(buf)-1)) > 0){
        buf[len] = '\0';
        if(buf[0] == '\0' || strcmp("exit", buf) == 0 || strcmp("quit", buf) == 0)
            break;

        clientlist_printf_msg("\x1b[1;32m%s\x1b[0m: %s\n", name, buf);
    }
    if(len < 0){
        fprintf(stderr, "failed to write to the client: %s\n", strerror(errno));
    }

    clientlist_printf_msg("\x1b[1;32m%s\x1b[0m exited the chat\n", name);
    clientlist_erase(name); //close file descriptor and erase clientnode
    clientlist_debug();
}

/*
static int receive_message(int clientfd, char* buf, int buflen){
    return 0;
}
*/

static int get_user_name(int clientfd, char* name, int namesz){
    while(1){
        namesz = read(clientfd, name, sizeof(name)-1); //write 1
        if(namesz < 0){
            return -1;
        }
        name[namesz] = '\0';
        if(clientlist_is_busy(name)){
            if(write(clientfd, "F", 1) <0)
                return -1;
            continue;
        }
        if(write(clientfd, "S", 1) < 0)
            return -1;
        clientlist_add(name, clientfd);
        clientlist_debug();
        break;
    }
    return namesz;
}