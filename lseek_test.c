#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define MSG "hello!\n"

void err(const char* msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(void) {
    __mode_t old_mask = umask(0);
    int fd = open("seek_file", O_TRUNC | O_CREAT | O_RDWR, 0777);
    if(fd == -1)
        err("Failed to create a file");

    if(lseek(fd, +16, SEEK_END) == -1)
        err("lseek()");
    printf("sizeof() = %lu\n", sizeof(MSG));
    printf("strlen() = %lu\n", strlen(MSG));

    if(write(fd,MSG, strlen(MSG)) == -1)
        err("write()");

    if(close(fd) == -1)
        err("Failed to close the file");
    umask(old_mask);
    return 0;
}