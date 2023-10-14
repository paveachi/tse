# Makefile for Crawler 
# Paul Chirkov

L1 = ../libcs50
L2 = ../common

OBJS = crawler.o
LIBS = 
LLIBS = $(L1)/libcs50-given.a $(L2)/common.a 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$(L1) -I$(L2)
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

crawler: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@
#crawler.o dependsd on h files
crawler.o: $(L1)/webpage.h $(L2)/pagedir.h $(L1)/hashtable.h $(L1)/bag.h  

.PHONY: test valgrind clean

# expects a file `test.names` to exist; it can contain any text.
test: testing.sh
	./testing.sh 

valgrind: testing.sh
	$(VALGRIND) ./testing.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
	rm -f vgcore.*
	rm -r ../data/*