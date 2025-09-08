CFLAGS:=-g
CC:=gcc

BUILDDIR:=build

SRC:=$(shell ls *.c)

SRC:=$(filter-out utils.c, $(SRC))
OUT:=$(patsubst %.c, $(BUILDDIR)/%.out, $(SRC))
OBJ:=$(BUILDDIR)/utils.o

all: $(OBJ) $(OUT)

$(OBJ): utils.c utils.h
	mkdir -p $(BUILDDIR)
	$(CC) -o $@ utils.c -c

$(BUILDDIR)/%.out: %.c
	$(CC) -o $@ $^ $(OBJ)

clean:
	rm -f build/*

.PHONY: clean