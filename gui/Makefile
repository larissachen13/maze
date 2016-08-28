# Makefile for gui
#
# Benji Hannam and Benjamin Littlejohn, August 2016
# Team core_dumped_in_a_maze

OBJS = mazedrawer.o 
LIB = gui.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb `pkg-config --cflags --libs gtk+-2.0` -lpthread
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	ar cr $@ $^

mazedrawer.o: mazedrawer.h

# add cleanup commands for other subdirectories
.PHONY: clean

clean:
	rm -f *~
	rm -f *.o
	rm -f $(LIB)