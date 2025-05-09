CFLAGS=-Wall -Wextra -g

all: chatserv chatclient clientlist.o

clientlist.o: clientlist.c clientlist.h
	gcc -c clientlist.c -o clientlist.o -g

chatserv: chatserv.c utils.h clientlist.o
	gcc chatserv.c clientlist.o -o chatserv $(CFLAGS)  

chatclient: chatclient.c utils.h
	gcc chatclient.c -o chatclient $(CFLAGS)

clean:
	rm -f chatclient chatserv clientlist.o