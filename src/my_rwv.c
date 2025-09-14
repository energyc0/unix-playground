#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* io list */
struct iol {
    void* data;
    size_t n;
    struct iol* next;
};

void* safe_malloc(unsigned int n)
{
    void* ptr = malloc(n);
    if (ptr == NULL) {
        perror_quit("malloc()");
    }
    return ptr;
}

struct iol* iol_alloc(void* data, size_t n)
{
    struct iol* ptr = safe_malloc(sizeof(struct iol));
    ptr->data = data;
    ptr->n = n;
    ptr->next = NULL;
    return ptr;
}

void iol_free(struct iol* p)
{
    while (p) {
        struct iol* temp = p->next;
        free(p);
        p = temp;
    }
}

ssize_t my_readv(int fd, struct iol* data)
{
    size_t sz = 0;
    for (const struct iol* ptr = data; ptr; ptr = ptr->next)
        sz += ptr->n;

    char* buf = safe_malloc(sz);

    ssize_t ret = read(fd, buf, sz);
    if (ret > 0) {
        size_t offset = 0;
        for (struct iol* ptr = data; ptr; ptr = ptr->next) {
            memcpy(ptr->data, buf + offset, ptr->n);
            offset += ptr->n;
        }
    }

    free(buf);
    return ret;
}
ssize_t my_writev(int fd, const struct iol* data)
{
    size_t sz = 0;
    for (const struct iol* ptr = data; ptr; ptr = ptr->next)
        sz += ptr->n;

    char* buf = safe_malloc(sz);
    size_t offset = 0;
    for (const struct iol* ptr = data; ptr; ptr = ptr->next) {
        memcpy(buf + offset, ptr->data, ptr->n);
        offset += ptr->n;
    }

    ssize_t ret = write(fd, buf, sz);
    free(buf);
    return ret;
}

struct iol* charlist_to_iol(char** argv)
{
    struct iol* ptr = NULL;
    struct iol* root = NULL;
    for (; *argv; argv++) {
        if (root == NULL) {
            root = ptr = iol_alloc(*argv, strlen(*argv));
        } else {
            ptr->next = iol_alloc(*argv, strlen(*argv));
            ptr = ptr->next;
        }
    }
    return root;
}

struct iol* copy_iol_size(const struct iol* list)
{
    struct iol* root = NULL;
    struct iol* root_ptr = NULL;
    for (const struct iol* list_ptr = list; list_ptr; list_ptr = list_ptr->next) {
        char* temp = safe_malloc(list_ptr->n);
        if (root == NULL) {
            root = root_ptr = iol_alloc(temp, list_ptr->n);
        } else {
            root_ptr->next = iol_alloc(temp, list_ptr->n);
            root_ptr = root_ptr->next;
        }
    }
    return root;
}

int main(int argc, char** argv)
{
    if (argc < 3)
        fatal("Usage: %s filename string1 [string2] ...\n"
              "Create file with 'filename' and write strings there via one write() call\n",
            argv[0]);

    struct iol* list = charlist_to_iol(argv + 2);
    int fd = open(argv[1], O_RDWR | O_TRUNC | O_CREAT, 0664);
    if (fd == -1)
        perror_quit("open()");

    if (my_writev(fd, list) == -1)
        perror_quit("my_writev()");

    struct iol* readlist = copy_iol_size(list);
    if (lseek(fd, 0, SEEK_SET) == -1)
        perror_quit("lseek()");

    my_readv(fd, readlist);
    for (struct iol* ptr = readlist; ptr; ptr = ptr->next) {
        printf("%.*s\n", (int)ptr->n, (char*)ptr->data);
        free(ptr->data);
    }

    close(fd);
    iol_free(readlist);
    iol_free(list);
}