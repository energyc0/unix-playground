#include <stdio.h>
#include <stdlib.h>

#define eprintf(...) do{ \
    fprintf(stderr, __VA_ARGS__); \
    exit(EXIT_FAILURE); \
}while(0)

void process_arg(const char* arg);
void grepping();