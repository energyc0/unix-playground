#include "utils.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define TESTFILE "file"

__off_t fd_off_cmp(int fd1, int fd2)
{
#define is_valid(offset)            \
    do {                            \
        if (offset == -1) {         \
            perror("fd_off_cmp()"); \
            return -1;              \
        }                           \
    } while (0)

    __off_t off1 = lseek(fd1, 0, SEEK_CUR);
    is_valid(off1);

    __off_t off2 = lseek(fd2, 0, SEEK_CUR);
    is_valid(off2);

    errno = 0;
    return off1 - off2;

#undef is_valid
}

int fd_flg_cmp(int f1, int f2)
{
#define is_valid(flags)             \
    do {                            \
        if (flags == -1) {          \
            perror("fd_flg_cmp()"); \
            return -1;              \
        }                           \
    } while (0)

    int flags1 = fcntl(f1, F_GETFL);
    is_valid(flags1);

    int flags2 = fcntl(f2, F_GETFL);
    is_valid(flags2);

    return flags1 == flags2;
}

int main(void)
{
    int fd1 = open(TESTFILE, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    int fd2 = dup(fd1);
    int fd3 = open(TESTFILE, O_RDWR);
    int fd4 = dup(fd3);
    int fd5 = open(TESTFILE, O_WRONLY, 0644);

    const char buf[] = "message";
    if (write(fd1, buf, strlen(buf)) == -1)
        perror_quit("write()");

    __off_t off_res;
    printf("fd_off_cmp(fd1, fd2) = %ld\n", (long)(off_res = fd_off_cmp(fd1, fd2)));
    printf("fd_off_cmp(fd1, fd3) = %ld\n", (long)(off_res = fd_off_cmp(fd1, fd3)));
    printf("fd_off_cmp(fd1, 10) = %ld\n", (long)(off_res = fd_off_cmp(fd1, 10)));
    printf("fd_off_cmp(fd4, fd3) = %ld\n", (long)(off_res = fd_off_cmp(fd4, fd3)));
    printf("fd_off_cmp(fd1, fd5) = %ld\n", (long)(off_res = fd_off_cmp(fd1, fd5)));

    int flg_res;
    printf("\nfd_flg_cmp(fd1, fd5) = %d\n", (flg_res = fd_flg_cmp(fd1, fd5)));
    printf("fd_flg_cmp(fd1, 10) = %d\n", (flg_res = fd_flg_cmp(fd1, 10)));
    printf("fd_flg_cmp(fd1, fd3) = %d\n", (flg_res = fd_flg_cmp(fd1, fd3)));
    printf("fd_flg_cmp(fd4, fd3) = %d\n", (flg_res = fd_flg_cmp(fd4, fd3)));

    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);
    close(fd5);
    return 0;
}