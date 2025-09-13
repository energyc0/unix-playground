#include <stdio.h>
#include "utils.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MSG_SIZE (16)

int main(void)
{
    int fd = open("file_append", O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0664);
    if (fd == -1)
        perror_quit("open()");

    char buf[MSG_SIZE];
    memset(buf, '0', sizeof(buf));

    if (write(fd, buf, sizeof(buf)) == -1)
        perror_quit("write()");

    if (lseek(fd, 0, SEEK_SET) == -1)
        perror_quit("lseek()");

    memset(buf, '1', sizeof(buf));
    if(write(fd,buf,sizeof(buf)) == -1)
        perror_quit("write()");

    if (close(fd) == -1)
        perror_quit("close()");
    return 0;
}