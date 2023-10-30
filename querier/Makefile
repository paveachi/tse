# Makefile for indexer 
# Paul Chirkov

all: fuzzquery querier

L1 = ../libcs50
L2 = ../common

OBJS = querier.o
LIBS = 
LLIBS = $(L1)/libcs50-given.a $(L2)/common.a 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$(L1) -I$(L2)
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

querier: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@
querier.o: $(L1)/webpage.h $(L2)/pagedir.h $(L2)/index.h $(L2)/word.h $(L1)/counters.h $(L1)/file.h

fuzzquery: fuzzquery.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@
fuzzquery.o: $(L1)/file.h $(L1)/mem.h

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
	rm -f querier 
	rm -f fuzzquery 

