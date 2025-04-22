#include "filelist.h"
#include "my_grep.h"
#include <errno.h>
#include <string.h>

struct filenode{
    char* filename;
    struct filenode* next;
};

static struct filenode* makenode(const char* fname);
static void freenode(struct filenode* node);
static void freelist(struct filenode* root);

static struct filelist{
    struct filenode* root;
    struct filenode* end;
}filelist;

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

void flist_add(const char* filename){
    if(filelist.root){
        filelist.end->next = makenode(filename);
        filelist.end = filelist.end->next;
    }
    else
        filelist.root = filelist.end = makenode(filename);
}

int flist_is_empty(){
    return filelist.root == NULL;
}

char* flist_get(){
    static struct filenode* p = NULL;
    if(p == NULL){
        if(filelist.root == NULL)
            return NULL;
        p = filelist.root;
    }
    if(p->next == NULL)
        return NULL;

    struct filenode* res = p;
    p = p->next;
    return res->filename;
}

void flist_free(){
    freelist(filelist.root);
    filelist.root = NULL;
    filelist.end = NULL;
}
