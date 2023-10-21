/* indexer.C 
 * 
 * A program for creating an inverted index of words from a crawler directory.
 *
 * Paul Chirkov October 2023
 */

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

/**************** main ****************/
int main(const int argc, char* argv[]){
    
    char* pageDirectory = NULL;
    char* indexFilename = NULL;

    //parse command line, validate parameters   
    parseArgs(argc, argv, &pageDirectory, &indexFilename);
    //build the index
    indexBuild(pageDirectory, indexFilename);
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
static void parseArgs(const int argc, char* argv[],  char** pageDirectory, char** indexFilename){

    const char* progName = argv[0]; // name of this program

    //ensure there are exactly 2 arguments
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
    FILE* file = fopen(argv[2], "w+");
    if(file == NULL){
        fprintf(stderr, "Not a valid path for the index file.\n");
        exit(3);
    }

    *indexFilename = argv[2];
    fclose(file);
}

/**************** indexBuild ****************/
/* creates new index and loops over pageDirectory files, calling indexPage() on each webpage.
* 
* Caller provides:
*   the crawler directory, the desired indexFilename
* We do:
*   Trys to load each webpage from crawler directory, add all words to index.
*/
void indexBuild(char* pageDirectory, char* indexFilename){
    
    //Creates new 'index' object, arbitrary size
    index_t* index = index_new(600);
    //loops over doc ID numbers, until undreadable file
    for(int id=1; ;id++){
        //loads webpage from document file 'pagedirectory/id'
        webpage_t* webpage = pagedir_load(pageDirectory, id);
        //if the page_dir is sucessful (file exists)
        if(webpage != NULL){
            //get the html and if sucessful
            if(webpage_fetch(webpage)){
                indexPage(webpage, id, index);
            }
        }
        else{
            break;
        }
        //delete webpage
        webpage_delete(webpage);
    }

    //save the index to a file
    index_save(index, indexFilename);
    //delete the index struct
    index_delete(index);
}


/**************** indexBuild ****************/
/* Loop through words in webpage and add it to index
* 
* Caller provides:
*   valid webpage, the docID and the index to add to.
* We do:
*   Ignore words of length less than three, normalize word and add to index.
*/
void indexPage(webpage_t* page, int docID, index_t* index){

    //steps through each word on webpage
    int pos = 0;
    char* word;
    while ((word = webpage_getNextWord(page, &pos)) != NULL) {

        //skips trivial words (less than len 3)
        if(strlen(word) >= 3){
            //normalizes word
            word_normalize(word);
            //add to word to index (increment or initialize to 1)
            index_add(index, word, docID);
        }
        free(word);
    }

}


