# CS 50 TSE indexer implementation spec

# Data structures

We create an 'index' data structure which stores the inverted index in memory. 

# Control flow

The indexer is implemented in one file `indexer.c` with three functions.

### main

The `main` function simply calls `parseArge` and `indexBuild`, then exits zero.

### parseArgs

Given the arguments from the command line, extract them into the function parameters, return only if sucessful. 

* ensure there are the correct number of arguments
* ensure `pageDirectory` is a valid directory using pagedir_validate().
* ensure `indexFilename` is a writeable file. 

### indexBuild

`indexBuild` create an in-memory index from the webapage files in the pageDirectory. Because it is impossible to know the size of the index in advance, it is initialized arbitrarily to 600. It calls `indexPage` to scan each given webpage and add each word to the index. The pseudocode is detailed in the design spec. 

### indexPage

`indexPage` reads all the words from the webpage and adds them to a given index. The pseudocode for this function is in the design spec.

## Other functions and modules

#### index.c 

This is the index data structure that represents an in-memory inverted index. It is implemented as mostly a wrapper around the "hashtable" data structure provided in libcs50. It is essentially a hashtable with word keys and `counter_t*` items. Functions exist to create new index, set counters, add to counters, load index from file, and save an index to a file. 

### pagedir.c
Two new functions added for the indexer. The first one is `pagedir_validate` which simply checks that a directory has a `.crawler` file by attempting to open a `.crawler` file to read in the given directory.
The second function is `pagedir_load` which reads a file in the crawler directory and creates a webpage object with the URL and depth. It does not read the html into the file and therefore it must be retrieved with `webpage_fetch()` if it is needed. When loading an index, the `pagedir_load` counts the number of lines for the index hashtable and sets its size to (1.3 * numWords). This allows performance to be reliable.  

### word.c
A module consisting of a single function for normalizing a word. It loops through the characters in a `char*` and calls the `tolower()` method on each one.

## Function protypes

### pagedir.c
The functions added to pagedir.c. Detailed description of each functions interface is provided in pagedir.h.

```c
bool pagedir_validate(char* pageDirectory);
webpage_t* pagedir_load(char* pageDirectory, int docID);

```

### index.c
The functions in index.c. Detailed description of each functions interface is provided in index.h.
```c
index_t* index_new(int size);
bool index_add(index_t* index, char* word, int docID);
bool index_set(index_t* index, char* word, int docID, int count);
void index_delete(index_t* index);
index_t* index_load(char* pathName);
```

### crawler.c 
The functions in crawler.c. Detailed description of each functions interface is provided in comments before each one.
```c
static void parseArgs(const int argc, char* argv[],char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
```

# Error handling and recovery
command line parameters are rigorously checked with `parseArgs()`. Defensive programming techniques are also used extensively and many functions ensure that the parameters they receive are not NULL or invalid before proceeding. Manytimes also errors are handled internally and functions will return NULL or false if they do not succeed.

# Testing plan

Run it with all different commands. The indexer is run in testing.sh with a variety of bad inputs to test parseargs(). Then the indexer is run on the crawler produced files and compared with the cs-50 example output to demonstrate that they are correct. 

Then, indextest is tested by created new index files from the old ones and then comparing them to ensure that they are in fact the same. 

