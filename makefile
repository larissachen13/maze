# Makefile for maze client program
#
# Larissa Chen August 2016
# Team core_dumped_in_a_maze

# programs
PROG = AMStartup 	# program w/ just ASCII GUI
GTK = AMStartupgtk 	# program w/ GTK GUI
SLEEP = AMStartupsleep # program w/ one second sleep

# .o files for each program
OBJS = AMStartup.o ./thread_ops/thread_ops.o
GTKOBJS = AMStartupgtk.o ./thread_ops/thread_ops.o
SLEEPOBJS = AMStartup.o tosleep.o

# libraries
LLIBS = ./lib/lib.a
GTKLIBS = ./gui/gui.a

# commands and switches
CC = gcc
MAKE = make
CFLAGS = -Wall -pedantic -std=c11 -ggdb -lpthread
GTKFLAGS = $(CFLAGS) -DGUI `pkg-config --cflags --libs gtk+-2.0`

# build the maze client program without graphics
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

# build the maze client program with graphics
$(GTK): $(GTKOBJS) $(LLIBS) $(GTKLIBS)
	$(CC) $(GTKFLAGS) $^ -o $@

# build the maze client program with one second sleep
$(SLEEP): $(SLEEPOBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

# source dependencies
AMStartup.o: ./thread_ops/thread_ops.h
./thread_ops/thread_ops.o: ./thread_ops/thread_ops.h ./lib/mazestruct.h

# source dependency for graphics version
AMStartupgtk.o: ./thread_ops/thread_ops.h ./gui/mazedrawer.h
	$(CC) $(GTKFLAGS) -c AMStartup.c -o $@

# source dependency for sleep version
tosleep.o: ./thread_ops/thread_ops.h ./gui/mazedrawer.h
	$(CC) $(CFLAGS) -DSLEEP -c ./thread_ops/thread_ops.c -o $@

# build the library
./lib/lib.a: ./lib/avatar.c ./lib/mazestruct.c
	cd ./lib; $(MAKE)

#build the gui library
./gui/gui.a: ./gui/mazedrawer.c
	cd ./gui; $(MAKE)

.PHONY: clean

clean:
	rm -f *~
	rm -f *.o
	rm -f *.log
	rm -f core.*
	rm -f vgcore.*
	rm -f $(PROG)
	rm -f AMStartupgtk
	rm -f AMStartupsleep
	rm -f thread_ops/*.o
	cd ./lib; $(MAKE) clean
	cd ./gui; $(MAKE) clean
