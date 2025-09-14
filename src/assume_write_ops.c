#include "utils.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define TESTFILE "file"

/*
 * Gidday world
 */

int main(void)
{
    int fd1 = open(TESTFILE, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    int fd2 = dup(fd1);
    int fd3 = open(TESTFILE, O_RDWR);

    write(fd1, "Hello,", 6);
    write(fd2, " world", 6);
    lseek(fd2, 0, SEEK_SET);
    write(fd1, "HELLO,", 6);
    write(fd3, "Gidday", 6);

    close(fd1);
    close(fd2);
    close(fd3);
}