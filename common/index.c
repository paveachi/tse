/* 
 * index.c - CS50 'index' module/data structure
 *
 * see index.h for more information.
 *
 * Paul Chirkov October 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <hashtable.h>
#include <counters.h>
#include <file.h>

/**************** global types ****************/
typedef struct index
{
    hashtable_t* hashtable;
}index_t;


/**************** local functions ****************/
void counterFunc(void* file, const char* key, void* counter);
void counterPrint(void* arg, int key, int count);
void deleteItem(void* item);


/**************** index_new ****************/
/* see index.h for description */
index_t* index_new(int size){
    //allocate memory for index 
    index_t* index = malloc(sizeof(index));
        if(index == NULL){
            return NULL; //memory allocation failed
        }
        index->hashtable = hashtable_new(size);
    return index;
}

/**************** index_add ****************/
/* see index.h for description */
bool index_add(index_t* index, char* word, int docID){
    if(index == NULL || word == NULL || docID <= 0){
        return false;
    }

    //create new counter
    counters_t* newCounter = counters_new();
    
    if(!hashtable_insert(index->hashtable, word, newCounter)){
        //if key already exists in table (and therefore counter), get rid of counter
        free(newCounter);
    }
    counters_t* counter = hashtable_find(index->hashtable, word);
    //find the counter and add to it.
    if(counters_add(counter, docID) > 0){
        return true;
    }
    return false;
}

/**************** index_set ****************/
/* see index.h for description */
bool index_set(index_t* index, char* word, int docID, int count){

    if(index == NULL || word == NULL || docID <= 0 || count < 0){
        return false;
    }

    //create new counter
    counters_t* newCounter = counters_new();

    if(!hashtable_insert(index->hashtable, word, newCounter)){
        //if key already exists in table (and therefore counter), get rid of counter
        free(newCounter);
    }
    counters_t* counter = hashtable_find(index->hashtable, word); //find counter
    //set the count
    if(counters_set(counter, docID, count)){
        return true;
    }
    return false;
}


/**************** index_delete ****************/
/* see index.h for description */
void index_delete(index_t* index){
    if(index != NULL){
        //delelete the hashtable.
        hashtable_delete(index->hashtable, deleteItem);
        //delete the index
        free(index);
    }
}

/**************** index_save ****************/
/* see index.h for description */
void index_save(index_t* index, char* pathName){
    
    //check that file is writeable
    FILE* file = fopen(pathName, "w+");
    if(file == NULL){
        return;
    }
    //write to file (see local functions)
    hashtable_iterate(index->hashtable, file, counterFunc);
    
    fclose(file);
}

/**************** index_load ****************/
/* see index.h for description */
index_t* index_load(char* pathName){
    
    // check file exists
    FILE* file = fopen(pathName, "r");
    if(file == NULL){
        return NULL; //error opening file (not valid path)
    }

    //count number of words (lines)
    int wordCount = (int)(file_numLines(file) * 1.3);
    //make new index with hashtable size 1.3* number of words.
    index_t* index = index_new(wordCount);

    //for each word
    for(int i=0; i<wordCount; i++){
        //read word
        char* word = file_readWord(file); 
        int id;
        int count;
        //read all the (docID, count) pairs
        while(fscanf(file, "%d %d ",&id, &count) == 2){ //Maybe fscanf needs to beFREE
            //add id, count to index struct.
            index_set(index, word, id, count);
        }
        free(word);
    }
    fclose(file);
    return index;
}


//function for interating through counters
void counterFunc(void* file, const char* key, void* counter){
    //print word
    fprintf(file, "%s ", key);
    //iterate through counters, print each one
    counters_iterate(counter, file, counterPrint);
    //add new line
    fprintf(file, "\n");
}

//function for printing each counter
void counterPrint(void* arg, int key, int count){
    fprintf(arg, "%d %d ", key, count);
}

//wrapper for counters_delete
void deleteItem(void* item){
    counters_delete(item);
}