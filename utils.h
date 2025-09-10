#ifndef UTILS_H
#define UTILS_H

#ifdef __GNUC__
    #define NORETURN __attribute__((noreturn))
#else
    #define NORETURN
#endif

void fatal(const char *fmt, ...) NORETURN;
void perror_quit(const char *msg) NORETURN;

typedef struct vector_t {
    int *buf;
    int capacity;
    int size;
} vector_t;

void vinit(vector_t *v);
void vpush_back(vector_t *v, int val);
int vpop_back(vector_t *v);
int vget(const vector_t *v, int i);
void vfree(vector_t *v);
void vgrow(vector_t *v);

#endif