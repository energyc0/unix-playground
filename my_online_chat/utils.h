#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#define FATAL(...) do{ \
    fprintf(stderr, __VA_ARGS__); \
    putc('\n', stderr); \
    exit(EXIT_FAILURE); \
}while(0)

#define CHATSERVER_PORT 7007
#define CHATSERVER_LISTENERS 10
#define CLIENTNAME_LEN 256

#define REQ_SUCCESS "S"
#define REQ_FAILED "F"

#endif