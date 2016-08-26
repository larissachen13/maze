# Makefile for 'lib' library
#
#
# Larissa Chen, Augst 2016

OBJS = avatar.o mazestruct.o
LIB = lib.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	ar cr $@ $^

avatar.o: avatar.h
mazestruct.o: mazestruct.h

# add cleanup commands for other subdirectories
.PHONY: clean

clean:
	rm -f *~
	rm -f *.o
	rm -f $(LIB)
