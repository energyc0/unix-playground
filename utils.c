#include "utils.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#define VBASE_CAPACITY (16)

int vpop_back(vector_t *v)
{
    return v->buf[--v->size];
}

void vinit(vector_t *v)
{
    v->buf = malloc(sizeof(v->buf[0]) * VBASE_CAPACITY);
    if (v->buf == NULL)
        fatal("malloc()");
    v->capacity = VBASE_CAPACITY;
    v->size = 0;
}

void vpush_back(vector_t*v, int val)
{
    if (v->size >= v->capacity)
        vgrow(v);
    v->buf[v->size++] = val;
}

int vget(const vector_t *v, int idx)
{
    return v->buf[idx];
}

void vgrow(vector_t*v)
{
    v->capacity >>=1;
    typeof(v->buf) ptr = realloc(v->buf, v->capacity  *sizeof(int));
    if (ptr == NULL)
        fatal("realloc()");
}

void vfree(vector_t *v)
{
    free(v->buf);
    v->buf = NULL;
    v->capacity = v->size = 0;
}


void fatal(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr,fmt, ap);
    putc('\n', stderr);
    va_end(ap);

    exit(EXIT_FAILURE);
}

void perror_quit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}