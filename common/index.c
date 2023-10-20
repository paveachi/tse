// functions index_new(), index_delete(), index_save() 
// read and write index files 

//mostly a wrapper for hashtable
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <hashtable.h>
#include <counters.h>
#include <file.h>

typedef struct index
{
    hashtable_t* hashtable;
}index_t;

void counterFunc(void* file, const char* key, void* counter);
void counterPrint(void* arg, int key, int count);
void deleteItem(void* item);

index_t* index_new(int size){
    // create new empty (?) hashtable
        index_t* index = malloc(sizeof(index));
        if(index == NULL){
            return NULL; //memory allocation failed
        }
        index->hashtable = hashtable_new(size);
        //HASHTABLE MUST BE FREED
    return index;
    //INDEX MUST BE FREED BY CALLIN INDEX_DELETE(); 
}

bool index_add(index_t* index, char* word, int docID){
    //Try to insert word with new counter into hashtable. If already exists will do nothin.
    hashtable_insert(index->hashtable, word, counters_new());
    counters_t* counter = hashtable_find(index->hashtable, word);
    if(counters_add(counter, docID) > 0){
        return true;
    }
    return false;
}

bool index_set(index_t* index, char* word, int docID, int count){
    //Try to insert word with new counter into hashtable. If already exists will do nothin.
    hashtable_insert(index->hashtable, word, counters_new());
    counters_t* counter = hashtable_find(index->hashtable, word);
    if(counters_set(counter, docID, count)){
        return true;
    }
    return false;
}
//index_set() function?


//Do I need to create function to delete counters?
void index_delete(index_t* index){
    //iterate through hashtable keys, free all the counters. Pass counterdelete to hashtable_delete()
    //delete hashtable and counters... this will FREE hashtable and counters
    hashtable_delete(index->hashtable, deleteItem);
    //free memory for index
    free(index);
}


void index_save(index_t* index, char* pathName){
    //This function creates the index file from the data structure
    //writes key 

    FILE* file = fopen(pathName, "w+");
    if(file == NULL){
        return;
    }
    //Iterators call iterators
    hashtable_iterate(index->hashtable, file, counterFunc);
    
    fclose(file);
}


index_t* index_load(char* pathName){
    //creates index data structure from some index file... returnst that index data structure.
    //does most of the work of indexTester. 
    FILE* file = fopen(pathName, "r");
    if(file == NULL){
        return NULL; //error opening file (not valid path)
    }
    //count number of words using count function
    int wordCount = file_numLines(file);
    //make new index with hashtable size 1.3* number of words...
    index_t* index = index_new(wordCount);
    //reads a file from pathname, makes a new index

    for(int i=0; i<wordCount; i++){
        //read word
        char* word = file_readWord(file); //MUST FREE WORD
        //loop call fscanf
        int id;
        int count;
        while(fscanf(file, "%d %d ",&id, &count)){ //Maybe fscanf needs to beFREE
            //add id, count to counters of 
            index_set(index, word, id, count);
        }
        free(word);
    }
    //FREE EVERYTHING NEEDED TO BE FREED.
    
    return index;
}


void counterFunc(void* file, const char* key, void* counter){
    //print wrod
    fprintf(file, "%s ", key);
    counters_iterate(counter, file, counterPrint);
    //new line for new word
    fprintf(file, "\n");
}

void counterPrint(void* arg, int key, int count){
    fprintf(arg, "%d %d ", key, count);
}

void deleteItem(void* item){
    counters_delete(item);
}