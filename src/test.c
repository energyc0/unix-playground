#include "utils.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MSG "hello!\n"

int main(int argc, char** argv)
{
    int fd = open("file", O_EXCL | O_CREAT | O_RDWR);
    if (fd == -1)
        perror_quit("open()");
    if (write(fd, MSG, strlen(MSG)) == -1)
        perror_quit("write()");
    if (close(fd) == -1)
        perror_quit("close()");
    printf("Done.\n");
    return 0;
}