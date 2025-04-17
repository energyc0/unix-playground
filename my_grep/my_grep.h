#include <stdio.h>
#include <stdlib.h>

#define eprintf(...) do{ \
    fprintf(stderr, __VA_ARGS__); \
    exit(EXIT_FAILURE); \
}while(0)

#define undefined_arg() eprintf("Usage: my_grep [OPTION]... PATTERNS [FILE]\nTry my_grep --help for more information\n")

void process_arg(const char* arg);
void grepping();