#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include <pagedir.h>
#include <webpage.h>
#include <index.h>
#include <word.h>

static void parseArgs(const int argc, char* argv[],  char** pageDirectory, char** indexFilename);
void indexBuild(char* pageDirectory, char* indexFilename);
void indexPage(webpage_t* page, int docID, index_t* index);


int main(const int argc, char* argv[]){
    
    char* pageDirectory = NULL;
    char* indexFilename = NULL;

    //parse command line, validate parameters   
    parseArgs(argc, argv, &pageDirectory, &indexFilename);
    //initialize other modules. not quite sure what this means.
    
    //INITIALIZE INDEX HERE??

    //call indexBuild with pageDirectory
    indexBuild(pageDirectory, indexFilename);
    
    //SAVE AND DELETE INDEX HERE??
}

static void parseArgs(const int argc, char* argv[],  char** pageDirectory, char** indexFilename){
    //validate the two parameters are valid.
    const char* progName = argv[0]; // name of this program
    if (argc != 3) {
        fprintf(stderr, "usage: %s pageDirectory indexFilename\n", progName);
        exit(1);
    }
    //check that pagedirectory is a crawler-produced directory (validate dir function)
    if(!pagedir_validate(argv[1])){
        fprintf(stderr, "The given pageDirectory is not valid. It needs to be a directory with a .crawler file.\n");
        exit(2);
    }
    *pageDirectory = argv[1];
    //check that indexFilename is a file that exist/can be written... This is just open with w+
    //FOPEN WIHT W+.
    FILE* file = fopen(argv[2], "w+");
    if(file == NULL){
        fprintf(stderr, "Not a valid path for the index file.\n");
        exit(3);
    }
    *indexFilename = argv[2];
    fclose(file);
}

void indexBuild(char* pageDirectory, char* indexFilename){
    
    //Creates new 'index' object
    index_t* index = index_new(400);
    //loops over doc ID numbers, counting from 1
    for(int id=1; ;id++){
        //loads webpage from document file 'pagedirectory/id'
        webpage_t* webpage = pagedir_load(pageDirectory, id);
        if(webpage != NULL){
            if(webpage_fetch(webpage)){
                indexPage(webpage, id, index);
            }
        }
        else{
            break;
        }
        //DELETE THE WEBPAGE AFTER IT IS DONE
        webpage_delete(webpage);
    }

    //DO I SAVE THE INDEX HERE???
    index_save(index, indexFilename);

    index_delete(index);
}



void indexPage(webpage_t* page, int docID, index_t* index){

    //steps through each word on webpage
    int pos = 0;
    char* word;
    while ((word = webpage_getNextWord(page, &pos)) != NULL) {

        //skips trivial words (less than len 3)
        if(strlen(word) >= 3){
            //normalizes word
            word_normalize(word);
            index_add(index, word, docID);

            //looks up word in index (if in index)
                //increment count of word 
            //if not in index
                //add word to index
        }
        free(word);
    }

}


