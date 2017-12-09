CC     = gcc
CFLAGS = -g -Wall -I$(IDIR)
LIBS   = -lm

IDIR   = ./inc
ODIR   = obj
MKDIR  = mkdir -p

_DEPS = msconf.h moss.h moss/array.h moss/hash.h moss/string.h
DEPS  = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJS = array.o string.o hash/djb.o hash/fnv.o hash/joaat.o hash/murmur.o hash/sdbm.o hash/xxhash.o
OBJS  = $(patsubst %,$(ODIR)/%,$(_OBJS))

.PHONY: all
.PHONY: clean
.PHONY: directories
.PHONY: doc

moss: obj/main.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

directories:
	$(MKDIR) $(ODIR)/hash

obj/main.o: main.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# tylko pliki biblioteki
library: directories moss

# wszystko
all: directories moss doc

# dokumentacja
doc:
	make -C ./doc html

# czyści wszystkie pliki zapisane w folderze obj
clean:
	rm -f $(ODIR)/*.o $(ODIR)/hash/*.o
