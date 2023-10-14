#!/bin/bash
#Author: Paul Chirkov
#Date: 10/13/23
#Bash script for testing crawler.c

#Testing with a bunch of bad inputs
#Wrong number of arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters 15 23
#Depth too high 
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters 15
#depth not a number
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters depthargument
#website not internal
./crawler https://github.com/CS50Fall2023 ../data/letters 0
#Everything wrong!
./crawler https://github.com/CS50Fall2023 ../data/fakedirectory 14 2
#directory doesn't exist
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/fakedirectory 2

echo ""
echo "Letters to depth 0" 
# Testing letters website to depths 0, 1
mkdir ../data/letters-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters-0 0

echo "Letters to depth 1" 
mkdir ../data/letters-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters-1 1

# Testing wikipedia website to depths 0,1
echo ""
echo "Wikipedia to depth 0" 
mkdir ../data/wikipedia-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia-0 0

echo "Wikipedia to depth 1" 
mkdir ../data/wikipedia-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia-1 1

# Testing toscrape to depths 0,1
echo ""
echo "toscrape to depth 0" 
mkdir ../data/toscrape-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/toscrape-0 0

echo "toscrape to depth 1" 
mkdir ../data/toscrape-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/toscrape-1 1