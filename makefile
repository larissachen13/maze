# Makefile for maze
#
# Larissa Chen August 2016
# Team core_dumped_in_a_maze

PROG = AMStartup
OBJS = AMStartup.o ./thread_ops/thread_ops.o
LLIBS = ./lib/lib.a ./gui/gui.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -lpthread `pkg-config --cflags --libs gtk+-2.0`
CC = gcc
MAKE = make

# build the crawler
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

# crawler source dependencies; add others as needed
AMStartup.o: ./thread_ops/thread_ops.h ./gui/mazedrawer.h
./thread_ops/thread_ops.o: ./thread_ops/thread_ops.h ./lib/mazestruct.h

# build the library
./lib/lib.a:
	cd ./lib; $(MAKE)

#build the gui library
./gui/gui.a:
	cd ./gui; $(MAKE)

.PHONY: clean

clean:
	rm -f *~
	rm -f *.o
	rm -f *.log
	rm -f core.*
	rm -f vgcore.*
	rm -f $(PROG)
	cd ./lib; $(MAKE) clean
	cd ./gui; $(MAKE) clean
