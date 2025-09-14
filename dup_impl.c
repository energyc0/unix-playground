/*
 * dup*() commands allocate new file descriptor, that
 * points to the same opened file descriptor as 'oldfd'.
 * For example, if you perform two consecutive writes with
 * two different file descriptors (one is allocated with
 * dup*()), then they won't overwrite each other.
 */

#include "utils.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define MSG_MYDUP "Hello from mydup branch\n"
#define MSG_DUP "Hello from dup branch\n"

int mydup(int oldfd) 
{
    int fd = fcntl(oldfd, F_DUPFD, 0);
    return fd;
}   

int mydup2(int oldfd, int newfd)
{
    int flags2 = fcntl(newfd, F_GETFL);
    if (oldfd == newfd) {
        if (flags2 != -1)
            return oldfd;
    } else {
        int flags1 = fcntl(oldfd, F_GETFL);
        if (flags1 != -1) {
            close(newfd);
            return fcntl(oldfd, F_DUPFD, newfd);
        }
    }
    errno = EBADF;
    return -1;
}

int mydup_branch()
{
    const char* msg = MSG_MYDUP;
    printf("int fd = mydup(STDOUT_FILENO)\n");
    int fd = mydup(STDOUT_FILENO);
    if (fd == -1)
        perror_quit("mydup()");

    if (write(STDOUT_FILENO, msg, strlen(msg)) == -1)
        perror_quit("write()");    
    if (write(fd, msg, strlen(msg)) == -1)
        perror_quit("write()");

    printf("close(fd)\n");
    if (close(fd) == -1)
        perror_quit("close()");
    printf("write(STDOUT_FILENO)\n");
    if (write(STDOUT_FILENO, msg, strlen(msg)) == -1)
        perror_quit("write()");

    int dup2_arg = 10;
    fd = mydup2(dup2_arg, dup2_arg);
    printf("mydup2(%d, %d) returned %d\n",dup2_arg,dup2_arg, fd);
    if (fd == -1)
        perror("mydup2()");
    else
        close(fd);

    dup2_arg = 1;
    fd = dup2(dup2_arg, dup2_arg);
    printf("mydup2(%d, %d) returned %d\n",dup2_arg,dup2_arg, fd);
    if (fd == -1)
        perror("mydup2()");

    fd = dup2(STDOUT_FILENO, STDERR_FILENO);
    if (fd == -1)
        perror_quit("mydup2()");
    else
        printf("fd = mydup2(STDOUT_FILENO, STDERR_FILENO) = %d\n", fd);

    printf("write(STDERR_FILENO)\n");
    if (write(STDERR_FILENO, msg, strlen(msg)) == -1)
        perror_quit("write()");
    
    return 0;
}

int dup_branch()
{
    const char* msg = MSG_DUP;
    printf("int fd = dup(STDOUT_FILENO)\n");
    int fd = dup(STDOUT_FILENO);
    if (fd == -1)
        perror_quit("dup()");

    if (write(STDOUT_FILENO, msg, strlen(msg)) == -1)
        perror_quit("write()");    
    if (write(fd, msg, strlen(msg)) == -1)
        perror_quit("write()");

    printf("close(fd)\n");
    if (close(fd) == -1)
        perror_quit("close()");
    printf("write(STDOUT_FILENO)\n");
    if (write(STDOUT_FILENO, msg, strlen(msg)) == -1)
        perror_quit("write()");

    int dup2_arg = 10;
    fd = dup2(dup2_arg, dup2_arg);
    printf("dup2(%d, %d) returned %d\n",dup2_arg,dup2_arg, fd);
    if (fd == -1)
        perror("dup2()");
    else
        close(fd);

    dup2_arg = 1;
    fd = dup2(dup2_arg, dup2_arg);
    printf("dup2(%d, %d) returned %d\n",dup2_arg,dup2_arg, fd);
    if (fd == -1)
        perror("dup2()");

    fd = dup2(STDOUT_FILENO, STDERR_FILENO);
    if (fd == -1)
        perror_quit("dup2()");
    else
        printf("fd = dup2(STDOUT_FILENO, STDERR_FILENO) = %d\n", fd);

    printf("write(STDERR_FILENO)\n");
    if (write(STDERR_FILENO, msg, strlen(msg)) == -1)
        perror_quit("write()");
    
    return 0;
}

int main(int argc, char** argv)
{
    if (argc != 2 
        || (strcmp("-a", argv[1]) != 0 
        && strcmp("-b", argv[1]) != 0))
        fatal("Usage: %s [-a or -b]\n"
            "-a\tuse mydup*() functions\n"
            "-b\tuse dup*() functions\n",
            argv[0]);
    
    if (strcmp("-a", argv[1]) == 0)
        return mydup_branch();
    else
        return dup_branch();
}