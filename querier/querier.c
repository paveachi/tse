/* querier.C 
 * 
 * A querier program written for CS50's TSE.
 *
 * Paul Chirkov October 2023
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include <pagedir.h>
#include <webpage.h>
#include <index.h>
#include <word.h>
#include <counters.h>
#include <file.h>

//Core querier functions
void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
void parseQuery(char* query, char*indexFilename, char* pageDirectory);
void validateQueryStruct(char** wordArray, int queryLen);
void tokenize(char* query, char** wordArray, int queryLen);
void calculateScores(char** query, char* indexPath, int queryLen, char* pageDirectory);
void rankScores(counters_t* ctrs, char* pageDirectory);

//Functions for intersecting/unioning counters
void countersUnion(counters_t* ctrs1, counters_t* ctrs2);
void countersIntersection(counters_t* ctrs1, counters_t* ctrs2);

//Helper functions for various iterators
void countersUnionHelper(void* ctrs1, const int key, const int count2);
void countersSameKeys(void* ctrs2, const int key, const int count1);
void countersIntersectionHelper(void* ctrs1, const int key, const int count1);
void rankHelper(void* arg, const int key, const int count);
void countMatches(void* arg, const int key, const int count);
//local types, for finding max node in counter data struct.
struct max{
        int maxKey;
        int maxCount;
};



/**************** main ****************/
int main(const int argc, char* argv[]){
    char* pageDirectory;
    char* indexFilename;
    
    //validate command line arguments
    parseArgs(argc, argv, &pageDirectory, &indexFilename);
    
    //Read line by line from stdin.     
    while(!feof(stdin)){
        printf("query?\n");
        //read query 
        char* query = file_readLine(stdin);
        //if query is just EOF, exit 0.
        if(feof(stdin)){
            exit(0);
        }
        //query!
        parseQuery(query, indexFilename, pageDirectory);
        free(query);
        printf("--------------------------\n");
    }
    exit(0);
}

/**************** parseArgs ****************/
/* Handles command line arguments, makes sure they are all valid. 
* 
* Caller provides:
*   num arguments, array of arguments, pointers to variables that should be updated by arguments
* We guarantee: 
*   If the function doesn't exit non-zero, the arguments are valid
* We return:
*   only return when succesful, otherwise exit non-zero
*/
void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename){
    
    //verifies there are 3 arguments
    if(argc != 3){
        fprintf(stderr, "must have 3 arguments.\n");
        exit(1);
    }

    //verifies the given directroy has a .crawler file in it.
    if(!pagedir_validate(argv[1])){
        fprintf(stderr, "The given pageDirectory is not valid. It needs to be a directory with a .crawler file.\n");
        exit(2);
    }
    *pageDirectory = argv[1];

    // check that indexFilename is a valid path (readable file).
    FILE* file = fopen(argv[2], "r");
    if(file == NULL){
        fprintf(stderr, "Not a valid path for the index file.\n");
        exit(3);
    }

    *indexFilename = argv[2];
    fclose(file);
    
}



/**************** parseQuery ****************/
/* Validates query is valid with validateQueryStruct(). Creates the wordarray with tokenize(). Then calls calculateScores() on the given query.
* 
* Caller provides:
*   the query, the indexFilename and the pageDirectory
* We guarantee: 
*   If the function doesn't exit non-zero the query is valid. 
*/
void parseQuery(char* query, char*indexFilename, char* pageDirectory){
    
    //check that there is in fact some query
    if(query == NULL || strlen(query) == 0){
        fprintf(stderr, "error, there is no query.\n");
        exit(2);
    }

    //verify only contains letters and spaces
    for(int i=0; query[i]; i++){
        if(!(isalpha(query[i]) || isspace(query[i]))){
            fprintf(stderr, "error, must only contain letters and spaces.\n");
            exit(2);
        }
    }

    //get the size of the query
    int queryLen = word_numWords(query);
    //allocate memory for the array of words in the query
    char** wordArray = calloc(queryLen, sizeof(char*));
    if(wordArray == NULL){
        exit(5); // error allocatig memory
    }
    // creat the word array with tokenize
    tokenize(query, wordArray, queryLen);
    //make sure the structure of the query is valid
    validateQueryStruct(wordArray, queryLen);
    //get the scores for the query, output them
    calculateScores(wordArray, indexFilename, queryLen, pageDirectory);
    //delete wordArray
    free(wordArray);
}


/**************** validateQueryStruct() ****************/
/* Validates query is valid. Normalizes all the words in the query.
* 
* Caller provides:
*   the array of query words. The length of the array. 
* We guarantee: 
*   If the function doesn't exit non-zero the structure of the query is valid. 
* Assumes:
*   Query has already been checked for non space/alpha characters.
*/
void validateQueryStruct(char** wordArray, int queryLen){

    //print clean query and normalize all of the words.
    printf( "The clean query: ");
    for(int i =0; i<queryLen; i++){
        if(wordArray[i] == NULL){
            break;
        }
        else{
            //normalize each word in query.
            word_normalize(wordArray[i]);
            printf( "%s ", wordArray[i]);
        }
    }
    printf("\n");

    //check that the query doesn't start with 'and' or 'or'
    if((strcmp(wordArray[0], "or") ==0) || (strcmp(wordArray[0], "and")==0)){
        fprintf(stderr, "error, query cannot start with 'and' or 'or\n");
        exit(3);
    }

    //check that the query doesn't end with 'and' or 'or'
    if((strcmp(wordArray[queryLen - 1], "or")==0) || (strcmp(wordArray[queryLen - 1], "and")==0)){
        fprintf(stderr, "error, query cannot end with 'and' or 'or\n");
        exit(3);
    }
    
    //check no two operators are adjacent
    bool prev = false;
    for(int i =0; i<queryLen; i++){
        if((strcmp(wordArray[i], "or")==0 )|| (strcmp(wordArray[i], "and")==0)){
            if(prev){
                fprintf(stderr, "error, two operators cannot be next to each other\n");
                exit(4);
            }
            else{
                prev = true;
            }
        }
        else{
            prev = false;
        }
    }
}


/**************** tokenize() ****************/
/* Create array of words from query
* 
* Caller provides:
*   the array created array, the query and the length of the query (num words)
* Assumes:
*   Query has already been checked for non space/alpha characters.
*/
void tokenize(char* query, char** wordArray, int queryLen){

    char* word = query;
    //loop for num words in query
    for(int i = 0; i < queryLen; i++){
        //move to start of a word (past spaces)
        while((!isalpha(*word)) && (*word!='\0')){
            word++;
        }
        if(*word == '\0'){
            break;
        }
        if(isalpha(*word)){
            //start word 
            wordArray[i] = word;
            // find where word ends
            char* rest = strchr(word, ' ');
            if(rest == NULL){
                //make sure newline character is not ending the word (for reading a file).
                rest = strchr(word, '\n');
                if(rest != NULL){
                    *rest = '\0';
                }
                break;
            }
            // set rest to be a null terminator..
            *rest = '\0';
            //move past null terminator.
            word = rest+1;
        }
        
    }
}


/**************** calculateScores ****************/
/* Gets the counters data structure from the given index, calculates the score for the query.
* 
* Caller provides:
*   the array of query words and its length. The path of the index file, pageDirectory.
* Note:
*   It calls rankScores() and therefore outputs the ranked queries.
*/
void calculateScores(char** query, char* indexPath, int queryLen, char* pageDirectory){
    //load the index
    index_t* index = index_load(indexPath);

    //initialize first word to start runningProd.
    char* prev = "and";
    //create new counter for the runningSum (outer loop)
    counters_t* runningSum = counters_new();
    //create variable for the andSequence(inner loop)
    counters_t* runningProd;
    //loop through the array of queries.
    for(int i =0; i < queryLen; i++){
        //if word is 'and'
        if(strcmp(query[i], "and") == 0){
            prev = query[i];
            //set it to prev flag, move on
            continue;
        }
        if(strcmp(query[i], "or") == 0){
            prev = query[i];
            continue;
        }
        //get the counter of the current word
        counters_t* current = index_getCounters(index, query[i]);
        //for an 'or' sequence
        if(strcmp(prev, "or") == 0){
            // RunningSum union runningProd.
            countersUnion(runningSum, runningProd);
            // set runningProd to new set at word
            runningProd = current; 
        }
        //for an 'and' sequence
        else{
            // if this is the first word start the runningProd with it 
            if(i == 0){
                runningProd = current;
            }
            //if the current isn't null intersect them.
            if(current!= NULL){
                countersIntersection(runningProd, current);
            }
            //otherwise set the RunningProd to null
            else{
                runningProd = NULL;
            }
        }
        //update previous flag
        prev = query[i];
    }
    //Union the last Sum and Product.
    countersUnion(runningSum, runningProd); 

    //print out the number of matches. 
    int matches=0;
    counters_iterate(runningSum, &matches, countMatches);
    if(matches == 0){
        printf("Matches %d documents.\n", matches);
    }
    else{
        printf("Matches %d documents (ranked):\n", matches);
    }
    
    //rank all the scores
    rankScores(runningSum, pageDirectory);

    //delete INDEX, running prod
    index_delete(index);
    counters_delete(runningSum);
}



/**************** rankScores() ****************/
/* Ranks the documents in counter, prints them. 
* 
* Caller provides:
*   The counters object to rank, the crawler pageDirectory. 
* We do:
*   Print all Docs with non-zero scores.
*/
void rankScores(counters_t* ctrs, char* pageDirectory){
    // make sure ctrs is not null.
    if(ctrs != NULL){
        struct max maxCtr = {0,-1};

        while(maxCtr.maxCount != 0){
            maxCtr.maxCount = -1;
            maxCtr.maxKey = 0;
            //iterate through set and remove largerest
            counters_iterate(ctrs, &maxCtr, rankHelper);
            if(maxCtr.maxCount <= 0){
                break;
            }
            //if non-zero, get it's url and print it. 
            webpage_t* temp = pagedir_load(pageDirectory, maxCtr.maxKey);
            printf("Score: %d   doc:  %d   %s\n", maxCtr.maxCount, maxCtr.maxKey, webpage_getURL(temp));
            webpage_delete(temp);
            counters_set(ctrs, maxCtr.maxKey, 0);
        }
    }
}

/**************** countersUnion() ****************/
/* Sets the counts in ctrs1 to the sum of the counts in both ctrs1 and ctrs2
* 
* Caller provides:
*   The counter to be updated, the counter to be added.
* We do:
*   Loop through and sum the scores of the counter.
* Note: 
*   if ctrs2 is null, nothing is done to ctrs1.
*/
void countersUnion(counters_t* ctrs1, counters_t* ctrs2){
    counters_iterate(ctrs2, ctrs1, countersUnionHelper);
}

/**************** countersIntersection() ****************/
/* Sets the counts in ctrs1 to the min of corresponding nodes in the two ctrs.
* 
* Caller provides:
*   The counters to be updated, the counter to intersect it with.
* We do:
*   Ensure we don't miss something if the key in ctrs1 doesn't appear in ctrs2. 
*/
void countersIntersection(counters_t* ctrs1, counters_t* ctrs2){
    // loop through the ctrs1 and ensure the keys are the same (set key to zero in ctrs2 if it doesn't exist).
    counters_iterate(ctrs1, ctrs2, countersSameKeys);
    // loop through and set the count to the min between the two.. 
    counters_iterate(ctrs2, ctrs1, countersIntersectionHelper);
}




/*********Iterator helper functions **********/
/******************************************************/
//helper to get the largest node in the counter data structure.
void rankHelper(void* arg, const int key, const int count){
    struct max* max = arg;
    if(count > max->maxCount){
        max->maxKey = key;
        max->maxCount = count;
    }
}

// helper function, intersection of two countersets (and)
void countersUnionHelper(void* ctrs1, const int key, const int count2){
    int count1;
    if((count1 = counters_get(ctrs1, key)) != 0){
        counters_set(ctrs1, key, count2 + count1);
    }
    else{
        counters_set(ctrs1, key, count2);
    }
}

//intersection helper function to ensure the counters have the same keys
void countersSameKeys(void* ctrs2, const int key, const int count1){
    //loop through 1 if key not in 2 set key to 0 in 2. 
    //This makes sure we don't miss any when intersecting the sets...
    if(counters_get(ctrs2, key) == 0){
        counters_set(ctrs2, key, 0);
    }
}

//intersection helper, does the actual intersecting.
void countersIntersectionHelper(void* ctrs1, const int key, const int count1){
    //loop through a ctr and set count of each key in ctrs1 to min of the counts.
    int count2;
    if((count2 = counters_get(ctrs1, key)) != 0){
        if(count1 < count2){
            counters_set(ctrs1, key, count1);
        }
    }

}

//helper to count number of matches.
void countMatches(void* arg, const int key, const int count){
    int* matches = arg;
    if(count != 0){
        (*matches)++;
    }
}
