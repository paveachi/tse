#!/bin/bash
#Author: Paul Chirkov
#Date: 10/13/23
#Bash script for testing querier.c

# THIS SCRIPT ASSUMES EXISTENCE OF .crawler pagedirectory and .index files for wikipedia 0,1 and toscrape 0,1

#invalid command line arguments
#invalid index file
echo test | ./querier ../data/wikipedia-1 ../data/wikipedia
#invalid pagedirectory
echo test | ./querier ../data/ayothisatest ../data/wikipedia
#incorrect num of arguments
echo test | ./querier ../data/wikipedia-1 ../data/wikipedia 5 6 

#invalid queries
#starts with operator
echo and test | ./querier ../data/wikipedia-1 ../data/wikipedia-1.index
#end with operator
echo test and | ./querier ../data/wikipedia-1 ../data/wikipedia-1.index
#adjacent operators
echo this or and test | ./querier ../data/wikipedia-1 ../data/wikipedia-1.index
#invalid characters
echo test.1 | ./querier ../data/wikipedia-1 ../data/wikipedia-1.index

#testing valid queries
#one word
echo test | ./querier ../data/wikipedia-1 ../data/wikipedia-1.index
#and sequence
echo cat and mouse | ./querier ../data/wikipedia-1 ../data/wikipedia-1.index
#and sequence (implicit)
echo cat mouse | ./querier ../data/wikipedia-1 ../data/wikipedia-1.index
#or sequence
echo cat or mouse | ./querier ../data/wikipedia-1 ../data/wikipedia-1.index

#both (with weird formatting)
cat this    AND     teSt ANd car     or MOUSE | ./querier ../data/wikipedia-1 ../data/wikipedia-1.index

#random word testing w/ fuzzquery
#wikipedia to depth 0
./fuzzquery ../data/wikipedia-0.index 10 0 | ./querier ../data/wikipedia-0 ../data/wikipedia-0.index
#wikipedia to depth 1
./fuzzquery ../data/wikipedia-1.index 10 0 | ./querier ../data/wikipedia-1 ../data/wikipedia-1.index
#toscrape to depth 0
./fuzzquery ../data/toscrape-0.index 10 0 | ./querier ../data/toscrape-0 ../data/toscrape-0.index
#toscrape to depth 1
./fuzzquery ../data/toscrape-1.index 10 0 | ./querier ../data/toscrape-1 ../data/toscrape-1.index
