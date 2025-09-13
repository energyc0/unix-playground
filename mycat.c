#include "utils.h"
#include <stdio.h>

void readfile(FILE* fp);

int main(int argc, char** argv)
{
    if (argc == 1) {
        readfile(stdin);
    } else {
        for (int i = 1; i < argc; i++) {
            FILE* fp = fopen(argv[i], "r");
            if (fp == NULL) {
                perror(argv[i]);
            } else {
                readfile(fp);
                fclose(fp);
            }
        }
    }
    return 0;
}

void readfile(FILE* fp)
{
    int c;
    while ((c = getc(fp)) != EOF)
        putchar(c);
}