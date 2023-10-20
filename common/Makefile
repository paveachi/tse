# Makefile for CS50 common library
#   Builds and compiles all these utility functions into a library.
#
# Paul Chirkov October 2023

L = ../libcs50
# object files, and the target library
OBJS = pagedir.o index.o word.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
CC = gcc
MAKE = make

# Build $(LIB) by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h $L/webpage.h
index.o: index.h $L/hashtable.h $L/counters.h $L/file.h
word.o: word.h 

.PHONY: clean sourcelist

# clean up after compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
