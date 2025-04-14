#include "my_grep.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static char* pattern = NULL;

struct filenode{
    char* filename;
    struct filenode* next;
};

static struct filenode* list_root = NULL;
static struct filenode* list_end = NULL;

static struct filenode* makenode(const char* fname);
static void freenode(struct filenode* node);
static void freelist(struct filenode* root);

static void seek_pattern(FILE* fp);

void process_arg(const char *arg){
    if(strcmp(arg, "--help") == 0){
        printf("Usage: my_grep [OPTION]... PATTERNS [FILE]\n");
        exit(EXIT_SUCCESS);
    }else if(arg[0] == '-'){
        eprintf("Undefined argument \"%s\"\n", arg);
    }else if(pattern == NULL){
        pattern = strdup(arg);
    }else{
        if(list_root){
            list_end->next = makenode(arg);
            list_end = list_end->next;
        }
        else
            list_root = list_end = makenode(arg);
    }
}

void grepping(){
    FILE* fp = NULL;
    if(!list_root){
        fp = stdin;
        seek_pattern(fp);
    }else{
        struct filenode* p = list_root;
        for(;p != NULL; p = p->next){
            if((fp = fopen(p->filename, "r")) == NULL){
                fprintf(stderr, "\"%s\" - failed to open: %s\n", p->filename, strerror(errno));
                continue;
            }
            seek_pattern(fp);
            fclose(fp);
        }
        freelist(list_root);
    }
}

static struct filenode* makenode(const char* fname){
    struct filenode* p = malloc(sizeof(struct filenode));
    if(p == NULL){
        eprintf("malloc: %s\n", strerror(errno));
    }
    p->filename = strdup(fname);
    p->next = NULL;
    return p;
}

static void freenode(struct filenode* node){
    if(node){
        free(node->filename);
        free(node);
    }
}

static void freelist(struct filenode* root){
    struct filenode* p = root;
    while(root){
        p = p->next;
        freenode(root);
        root = p;
    }
}

static void seek_pattern(FILE* fp){
    char buf[BUFSIZ];
    while(fgets(buf, sizeof(buf), fp)){
        printf("%s", buf);
    }
}