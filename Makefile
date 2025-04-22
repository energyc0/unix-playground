CC = gcc
CFLAGS = -Wall -Wextra -g

.PHONY: all
all: my_grep.out

my_grep.out: ./my_grep/my_grep.c ./my_grep/my_grep.h ./my_grep/my_grep_impl.c ./my_grep/filelist.c ./my_grep/filelist.h
	${CC} ${CFLAGS} -o my_grep.out ./my_grep/my_grep.c ./my_grep/my_grep_impl.c ./my_grep/filelist.c


.PHONY: clean
clean: 
	rm -f *.out