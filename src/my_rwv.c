#include "utils.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

ssize_t my_readv(int fd, struct iovec* iov, int iovcnt)
{
    if (iovcnt <= 0) {
        errno = EINVAL;
        return -1;
    }
    size_t sz = 0;
    for (int i = 0; i < iovcnt; i++)
        sz += iov[i].iov_len;

    char* buf = malloc(sz);
    if (buf == NULL) {
        errno = ENOMEM;
        return -1;
    }
    ssize_t ret = read(fd, buf, sz);

    size_t offset = 0;
    int i = 0;
    ssize_t temp = ret;
    while (temp > 0) {
        size_t to_read = temp > iov[i].iov_len ? iov[i].iov_len : temp;
        memcpy(iov[i++].iov_base, buf + offset, to_read);
        temp -= to_read;
        offset += to_read;
    }

    free(buf);
    return ret;
}

ssize_t my_writev(int fd, struct iovec* iov, int iovcnt)
{
    if (iovcnt <= 0) {
        errno = EINVAL;
        return -1;
    }
    size_t sz = 0;
    for (int i = 0; i < iovcnt; i++)
        sz += iov[i].iov_len;

    char* buf = malloc(sz);
    if (buf == NULL) {
        errno = ENOMEM;
        return -1;
    }

    size_t offset = 0;
    for (int i = 0; i < iovcnt; i++) {
        memcpy(buf + offset, iov[i].iov_base, iov[i].iov_len);
        offset += iov[i].iov_len;
    }

    ssize_t ret = write(fd, buf, sz);
    free(buf);
    return ret;
}

int main(int argc, char** argv)
{
    if (argc < 3)
        fatal("Usage: %s filename string1 [string2] ...\n"
              "Create file with 'filename' and write strings there via one write() call\n",
            argv[0]);

    int fd = open(argv[1], O_RDWR | O_TRUNC | O_CREAT, 0664);
    if (fd == -1)
        perror_quit("open()");

    const int iovcnt = argc - 2;
    struct iovec bufs[iovcnt];

    for (int i = 0; i < iovcnt; i++) {
        bufs[i].iov_base = argv[i + 2];
        bufs[i].iov_len = strlen(argv[i + 2]);
    }
    my_writev(fd, bufs, iovcnt);

    for (int i = 0; i < iovcnt; i++) {
        bufs[i].iov_base = malloc(bufs[i].iov_len);
        if (bufs[i].iov_base == NULL)
            perror_quit("malloc()");
    }
    if (lseek(fd, 0, SEEK_SET) == -1)
        perror_quit("lseek()");

    printf("my_readv() debug:\n");
    my_readv(fd, bufs, iovcnt);
    for (int i = 0; i < iovcnt; i++) {
        printf("%.*s ", (int)bufs[i].iov_len, (char*)bufs[i].iov_base);
        free(bufs[i].iov_base);
    }
    putchar('\n');

    if (my_writev(111, bufs, iovcnt) == -1)
        perror("my_writev()");
    if (my_readv(111, bufs, iovcnt) == -1)
        perror("my_readv()");
    if (my_writev(1, bufs, -1) == -1)
        perror("my_writev()");
    if (my_readv(1, bufs, -1) == -1)
        perror("my_readv()");
    close(fd);
}