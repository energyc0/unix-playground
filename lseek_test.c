#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include "utils.h"

#define MSG "hello!\n"

int main(int argc, char** argv) {
    if(argc < 4)
        fatal("Usage: %s [filename] [file hole size] [text]", argv[0]);

    __mode_t old_mask = umask(0);
    int fd = open(argv[1], O_TRUNC | O_CREAT | O_WRONLY, 0777);
    umask(old_mask);
    if(fd == -1)
        fatal("Failed to create a file");

    int hole_size = atoi(argv[2]);
    if(hole_size <= 0)
        fatal("Incorrect file hole size!");

    if(lseek(fd, hole_size, SEEK_END) == -1)
        fatal("lseek()");
    
    if(write(fd,argv[3], strlen(argv[3])) == -1)
        fatal("write()");

    if(close(fd) == -1)
        fatal("Failed to close the file");
    return 0;
}