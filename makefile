# Makefile for maze
#
# Larissa Chen August 2016

PROG = AMStartup
OBJS = AMStartup.o
# LLIBS = ../lib/lib.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

# build the crawler
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

# crawler source dependencies; add others as needed
AMStartup.o: amazing.h

# build the library
../lib/lib.a:
	cd ../lib; $(MAKE)

.PHONY: clean

clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG)
	cd ../lib; $(MAKE) clean
