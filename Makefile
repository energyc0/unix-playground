CFLAGS:=-g
CC:=gcc

BUILDDIR:=build
SRCDIR:=src

SRC:=$(shell ls $(SRCDIR)/*.c)
SRC:=$(patsubst $(SRCDIR)/%.c, %.c, $(SRC))
DO_NOT_COMPILE:=utils.c

SRC:=$(filter-out $(DO_NOT_COMPILE), $(SRC))

OUT:=$(patsubst %.c, $(BUILDDIR)/%.out, $(SRC))
OBJ:=$(BUILDDIR)/utils.o

all: $(OBJ) $(OUT)

$(BUILDDIR)/elf_to_read_file.out: $(SRCDIR)/elf_to_read_file.c
	gcc -o $@ $^

$(OBJ): $(SRCDIR)/utils.c $(SRCDIR)/utils.h
	mkdir -p $(BUILDDIR)
	$(CC) -o $@ $(SRCDIR)/utils.c -c $(CFLAGS)

$(BUILDDIR)/%.out: $(SRCDIR)/%.c
	$(CC) -o $@ $^ $(OBJ) $(CFLAGS)

clean:
	rm -f build/* file*

.PHONY: clean
