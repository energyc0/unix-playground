/*
 * $ ./build/atomic_append.out f1 10000000 & ./build/atomic_append.out f1 10000000
 * [1] 54992
 * [54993] Appending to f1...
 * [54992] Appending to f1...
 * [54993] Done
 * [54992] Done
 * [1]+  Done                    ./build/atomic_append.out f1 10000000
 * $ ./build/atomic_append.out f2 10000000 -x & ./build/atomic_append.out f2 10000000 -x
 * [1] 54974
 * [54974] Writing to f2...
 * [54975] Writing to f2...
 * [54974] Done
 * [54975] Done
 * [1]+  Done                    ./build/atomic_append.out f2 10000000 -x 
 * ls -l f1 f2
 * -rw-rw-r-- 1 energyc0 energyc0 20000000 Sep 13 20:50 f1
 * -rw-rw-r-- 1 energyc0 energyc0 10045056 Sep 13 20:50 f2
 */

#include <stdio.h>
#include "utils.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MSG_SIZE (16)

int main(int argc, char** argv)
{
    if (argc > 4 || argc < 3)
        fatal("Usage: %s filename num_bytes [-x]\n"
            "-x\tappend to the file without O_APPEND",
            argv[0]);

    int c = atoi(argv[2]);
    int is_append = !(argc == 4 && strcmp("-x", argv[3]) == 0);
    int flags = O_WRONLY  
    | O_CREAT 
    | (is_append ? O_APPEND : 0);

    int fd = open(argv[1], flags, 0664);
    if (fd == -1)
        perror_quit("open()");

    char buf[BUFSIZ];
    memset(buf, '0', sizeof(buf));

    printf("[%d] %s to %s...\n",
        getpid(), (is_append ? "Appending" : "Writing"), argv[1]);
    
    while (c > 0) {
        if (!is_append && lseek(fd, 0, SEEK_END) == -1)
            perror_quit("lseek()");  

        int k = c >= BUFSIZ ? BUFSIZ : c;
        if ((k = write(fd, buf, k)) == -1)
            perror_quit("write()");
        c -= k;
    }
    if (close(fd) == -1)
        perror_quit("close()");

    printf("[%d] Done\n", getpid());
    return 0;
}