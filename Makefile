CFLAGS:=-g
CC:=gcc

BUILDDIR:=build
SRCDIR:=src

SRC:=$(shell ls $(SRCDIR)/*.c)
SRC:=$(patsubst $(SRCDIR)/%.c, %.c, $(SRC))
SRC:=$(filter-out utils.c, $(SRC))

OUT:=$(patsubst %.c, $(BUILDDIR)/%.out, $(SRC))
OBJ:=$(BUILDDIR)/utils.o

all: $(OBJ) $(OUT)

$(OBJ): $(SRCDIR)/utils.c $(SRCDIR)/utils.h
	mkdir -p $(BUILDDIR)
	$(CC) -o $@ $(SRCDIR)/utils.c -c

$(BUILDDIR)/%.out: $(SRCDIR)/%.c
	$(CC) -o $@ $^ $(OBJ)

clean:
	rm -f build/* file*

.PHONY: clean
