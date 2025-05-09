CFLAGS=-Wall -Wextra -g

all: chatserv.out chatclient.out

chatserv.out: chatserv.c utils.h clientlist.c clientlist.h
	gcc chatserv.c clientlist.c -o chatserv.out $(CFLAGS)  

chatclient.out: chatclient.c
	gcc chatclient.c -o chatclient.out $(CFLAGS)