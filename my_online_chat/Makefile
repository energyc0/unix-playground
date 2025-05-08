CFLAGS=-Wall -Wextra -g

all: chatserv.out chatclient.out

chatserv.out: chatserv.c utils.h
	gcc chatserv.c -o chatserv.out $(CFLAGS)  

chatclient.out: chatclient.c
	gcc chatclient.c -o chatclient.out $(CFLAGS)