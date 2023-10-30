# Implementation Spec for Querier - Paul Chirkov (github: paulchir)


## Data Structures
We load an index data structure from the index file in order to retrieve the counters for a given word. 

## Function protypes

The core querier functions
```c
void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
void parseQuery(char* query, char*indexFilename, char* pageDirectory);
void validateQueryStruct(char** wordArray, int queryLen);
void tokenize(char* query, char** wordArray, int queryLen);
void calculateScores(char** query, char* indexPath, int queryLen, char* pageDirectory);
void rankScores(counters_t* ctrs, char* pageDirectory);
```
The utility functions (in querier.c)
```c
void countersUnion(counters_t* ctrs1, counters_t* ctrs2);
void countersIntersection(counters_t* ctrs1, counters_t* ctrs2);
```
Helper functions for iterators
```c
void countersUnionHelper(void* ctrs1, const int key, const int count2);
void countersSameKeys(void* ctrs2, const int key, const int count1);
void countersIntersectionHelper(void* ctrs1, const int key, const int count1);
void rankHelper(void* arg, const int key, const int count);
void countMatches(void* arg, const int key, const int count);
```

Other utility functions 
In index.c
```c
counters_t* index_getCounters(index_t* index, char* word);
```
In word.c
```c
int word_numWords(char* query);
```

## Pseudocode

parseQuery

        check query is only spaces and letters
        count number of words in query
        initialize array of words
        tokenize the query
        validate structure of query
        calculate scores of the query

validateQueryStruct

        check query doesn't start or end with operator
        check no two operators are adjacent
        exit non-zero if fails

tokenize

        Loop the number of words in the query
            set start of word to first letter character
            add the start to the word array
            set end of word to the next space
            move start past the end

calculateScores

        load the index from the pageDirectory
        initialize 'operator' variable 
        create a runningSum and RunningProd
        loop through the query
            get current counters object
            if the word is an operator 
                set op variable to word
                move to next word
            if op is 'or'
                union the runningProd and runningSUm
                reset runningProd
            else
                intersect runningProd and current
        union runingSum and runningProd

        rankScores on the resulting runningSum


rankScores
        declare a max struct 
        iterate through counter 
            initalize max struct
            find highest score, print it to stdin

countersUnion

        Loop through counter2 with parameter counter1
            set each key, count in counter1 to sum of the two counts

countersIntersection

        Loop through counter1 with parameter counter2
            make sure counter2 has all the keys that exist in counter 1 
        loop through counter2 with parameter counter1
            set count in counter1 to min between corresponding counts
        

# Specific implementation choices

## Word.c

- I chose to implement a function in word.c to count the number of words in a string so that I could then tokenize the query efficiently. This works well since it means the exact correct amount of memory is allcated for the array of words in the query, and makes it simple to step through the query. I also implemented another function in index.c to get the counters item for a given word. This allowed me to retrieve the counter for a word and then union/intersect it accordingly.

## Retrieving max of counter

- Retrieving the max node in a counter required keeping track of both the key and the count, and therefore I created a max struct to hold precisely those two things.
