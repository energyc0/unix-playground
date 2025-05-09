#include "clientlist.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct clientnode{
    char* name;
    int clientfd;
    struct clientnode* next;
} clientnode;

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

static clientnode* root = NULL;
static clientnode* listhead = NULL;

static clientnode* client_find(char* name);

static clientnode* client_makenode(char* name, int fd); // allocate new node
//static clientnode* client_insertnode(clientnode* root, clientnode* node); // insert node to the root->next
static clientnode* client_erasenode(clientnode* node); // free node and return node->next
static void client_freenode(clientnode* node); //free node and close fd

// allocate new node
static clientnode* client_makenode(char* name, int fd){
    clientnode* ptr = malloc(sizeof(clientnode));
    if(ptr == NULL)
        FATAL("malloc()");

    ptr->name = strdup(name);
    ptr->clientfd = fd;
    ptr->next = NULL;
    return ptr;
}
/*
// insert node to the root->next
static clientnode* client_insertnode(clientnode* root, clientnode* node){
    if(root){
        if(node){
            clientnode* temp = root->next;
            root->next = node;
            node->next = temp;
        }
    }else
        return node;
    return root;
}
*/

// free node and return node->next
static clientnode* client_erasenode(clientnode* node){
    if(node){
        clientnode* temp = node->next;
        client_freenode(node);
        return temp;
    }
    return NULL;
}
 //free node and close fd
static void client_freenode(clientnode* node){
    if(node){
        free(node->name);
        close(node->clientfd);
        free(node);
    }
}

static clientnode* client_find(char* name){
    clientnode* ptr = root;
    while(ptr && strcmp(ptr->name, name) != 0)
        ptr = ptr->next;
    return ptr;
}

void clientlist_add(char* name, int fd){
    pthread_mutex_lock(&clients_mutex);
    if(listhead){
        listhead->next = client_makenode(name, fd);
        listhead = listhead->next;
    }else{
        root = listhead = client_makenode(name, fd);
    }
    pthread_mutex_unlock(&clients_mutex);
}

void clientlist_erase(char* name){
    pthread_mutex_lock(&clients_mutex);
    if(root){
        if(strcmp(root->name, name) == 0){
            if(root == listhead)
                listhead = root->next;
            root = client_erasenode(root);
        }else{
            clientnode* ptr = root;
            while (ptr->next) {
                if(strcmp(ptr->next->name, name) == 0){
                    if(ptr->next == listhead)
                        listhead = ptr;
                    ptr->next = client_erasenode(ptr->next);
                    break;
                }
                ptr = ptr->next;
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}
int clientlist_is_busy(char* name){
    pthread_mutex_lock(&clients_mutex);
    int ret = client_find(name) != NULL;
    pthread_mutex_unlock(&clients_mutex);

    return ret;
}
void clientlist_clear(){
    pthread_mutex_lock(&clients_mutex);
    while(root)
        root = client_erasenode(root);
    pthread_mutex_unlock(&clients_mutex);
}

void clientlist_debug(){
    clientnode* ptr = root;
    while(ptr){
        printf("%s->", ptr->name);
        ptr = ptr->next;
    }
    printf("NULL\n");
}