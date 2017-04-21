IDIR=./inc
CC=gcc
CFLAGS=-g -Wall -I$(IDIR)
ODIR=obj
LIBS=-lm
MKDIR=mkdir -p

_DEPS=msconf.h moss.h moss/array.h moss/buffer.h moss/hash.h moss/string.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJS=array.o buffer.o string.o hash/djb.o hash/fnv.o hash/joaat.o hash/murmur.o hash/sdbm.o hash/xxhash.o
OBJS=$(patsubst %,$(ODIR)/%,$(_OBJS))

.PHONY: all
.PHONY: clean
.PHONY: directories

moss: obj/main.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

directories:
	$(MKDIR) $(ODIR)/hash

obj/main.o: main.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: directories moss

clean:
	rm -f $(ODIR)/*.o $(ODIR)/hash/*.o
