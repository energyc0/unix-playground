CFLAGS=-g
CC=gcc

BUILDDIR=build

SRC=$(shell ls *.c)
OBJ=$(patsubst %.c, $(BUILDDIR)/%.out, $(SRC))

all: $(OBJ)

$(BUILDDIR)/%.out: %.c
	mkdir -p $(BUILDDIR)
	$(CC) -o $@ $^

clean:
	rm -f build/*

.PHONY: clean