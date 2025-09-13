/*
 * open() returns a new file descriptor to a new opened file descriptor
 * dup() returns a new file descriptor to the same opened file descriptor
 */

#include <float.h>
#include <stdio.h>
#include "utils.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MSG1 "FIRST\n"
#define MSG2 "SECOND\n"

void write_and_close(int fd1, int fd2);
int open_file(const char *filename);

int main(int argc, char** argv)
{
    int fd1 = open_file("file_dup");
    int fd2 = dup(fd1);
    if (fd2 == -1)
        perror_quit("dup()");

    write_and_close(fd1,fd2);

    fd1 = open_file("file_open");
    fd2 = open_file("file_open");
    write_and_close(fd1,fd2);

    return 0;
}

void write_and_close(int fd1, int fd2) 
{
    if (write(fd1, MSG1, strlen(MSG1)) == -1
        || write(fd2, MSG2, strlen(MSG2)) == -1)
        perror_quit("write()");

    if (close(fd1) == -1 || close(fd2) == -1)
        perror_quit("close()");
}

int open_file(const char *filename)
{
    int fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0777);
    if (fd == -1)
        perror_quit("open()");
    return fd;
}