#include "utils.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

#define failed_to_open(str) fatal("Failed to open '%s': %s\n", str, strerror(errno))

int main(int argc, char** argv) {
    if(argc != 3)
        fatal("Usage: %s [source file] [destination file]\n");
    
    int srcfd = open(argv[1], O_RDONLY);
    if(srcfd == -1)
        failed_to_open(argv[1]);

    mode_t oldmode = umask(0000);
    int dstfd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0664);
    umask(oldmode);
    if(dstfd == -1)
        failed_to_open(argv[2]);

    char buf[BUFSIZ];
    int c;
    while((c = read(srcfd, buf, BUFSIZ)) > 0){
        if(write(dstfd, buf, c) == -1)
            fatal("write(): %s", strerror(errno));
    }
    if(c == -1)
        fatal("read(): %s", strerror(errno));
    if(close(srcfd) == -1)
        perror("close()");
    if(close(dstfd) == -1)
        perror("close()");
    return 0;
}