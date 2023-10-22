#!/bin/bash
#Author: Paul Chirkov
#Date: 10/13/23
#Bash script for testing indexer.c and indextest.c
#Testing assumes 

#various invalid arguments
#no arguments
./indexer 
#one argument 
./indexer ../data/letters-0 
#three+ arguments
./indexer ../data/letters-0 ../data/letters-0.index yo

#invalid page directory non existent
./indexer yo ../data/letters-0.index

#invalid page directory not crawler
./indexer ../data ../data/letters-0.index

#invalid index file non existent path
./indexer ../data/letters-0 ../yo/

#testing indexer on .crawler directories that exist (could also use shared examples)
./indexer ../data/letters-0 ../data/letters-0.index
./indexer ../data/letters-1 ../data/letters-1.index

./indexer ../data/wikipedia-0 ../data/wikipedia-0.index
./indexer ../data/wikipedia-1 ../data/wikipedia-1.index

./indexer ../data/toscrape-0 ../data/toscrape-0.index

#comparing with example output to show that it worked!
~/cs50-dev/shared/tse/indexcmp ~/cs50-dev/shared/tse/output/letters-0.index ../data/letters-0.index
~/cs50-dev/shared/tse/indexcmp ~/cs50-dev/shared/tse/output/letters-1.index ../data/letters-1.index

# The example files for wikipedia are not correct! Therefore I have commented this out.
#~/cs50-dev/shared/tse/indexcmp ~/cs50-dev/shared/tse/output/wikipedia-0.index ../data/wikipedia-0.index
#~/cs50-dev/shared/tse/indexcmp ~/cs50-dev/shared/tse/output/wikipedia-1.index ../data/wikipedia-1.index

~/cs50-dev/shared/tse/indexcmp ~/cs50-dev/shared/tse/output/toscrape-0.index ../data/toscrape-0.index

#indextest
./indextest ../data/letters-0.index ../data/letters-0.indextest
./indextest ../data/letters-1.index ../data/letters-1.indextest 

./indextest ../data/wikipedia-0.index ../data/wikipedia-0.indextest
./indextest ../data/wikipedia-1.index ../data/wikipedia-1.indextest 

./indextest ../data/toscrape-0.index ../data/toscrape-0.indextest

#comparing the original index files to ensure they are the same
~/cs50-dev/shared/tse/indexcmp ../data/letters-0.index ../data/letters-0.indextest
~/cs50-dev/shared/tse/indexcmp ../data/letters-1.index ../data/letters-1.indextest 

~/cs50-dev/shared/tse/indexcmp ../data/wikipedia-0.index ../data/wikipedia-0.indextest
~/cs50-dev/shared/tse/indexcmp ../data/wikipedia-1.index ../data/wikipedia-1.indextest

~/cs50-dev/shared/tse/indexcmp ../data/toscrape-0.index ../data/toscrape-0.indextest