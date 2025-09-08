#ifndef UTILS_H
#define UTILS_H

#ifdef __GNUC__
    #define NORETURN __attribute__((noreturn))
#else
    #define NORETURN
#endif

void fatal(const char*, ...) NORETURN;

typedef struct vector_t{
    int* buf;
    int capacity;
    int size;
} vector_t;

void vinit(vector_t*);
void vpush_back(vector_t*, int);
int vpop_back(vector_t*);
int vget(const vector_t*, int);
void vfree(vector_t* v);
void vgrow(vector_t*v);

#endif