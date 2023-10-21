# Makefile for indexer 
# Paul Chirkov

all: indexer indextest

L1 = ../libcs50
L2 = ../common

OBJS1 = indexer.o
OBJS2 = indextest.o
LIBS = 
LLIBS = $(L1)/libcs50-given.a $(L2)/common.a 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$(L1) -I$(L2)
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

indexer: $(OBJS1) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@
indexer.o: $(L1)/webpage.h $(L2)/pagedir.h $(L2)/index.h $(L2)/word.h  

indextest: $(OBJS2) $(LLIBS) 
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@
indextest.o: $(L2)/pagedir.h $(L2)/index.h 

.PHONY: test valgrind clean

#testing command
test: testing.sh
	./testing.sh 

valgrind: testing.sh
	$(VALGRIND) ./testing.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
	rm -f vgcore.*
	rm -f ../data/*.index
