/* indextest.C 
 * 
 * Program for testing loading index struct from file.
 * 
 * Paul Chirkov October 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <index.h>
#include <pagedir.h>

/**************** main ****************/
int main(const int argc, char* argv[]){

    const char* progName = argv[0];

    if(argc != 3){
        fprintf(stderr, "Usage: %s oldIndexFilename newIndexFilename.\n", progName);
        exit(1);
    }

    char* oldIndexFilename = argv[1];
    char* newIndexFilename = argv[2];

    //check if oldIndexFile exists
    FILE* oldIndex = fopen(oldIndexFilename, "r");
    if(oldIndex == NULL){
        fprintf(stderr, "Error: oldIndexfile does not exists or is not readable.\n");            exit(2);
    }
    // load an index from a file
    index_t* index = index_load(oldIndexFilename);

    //check if newIndexFile exists or is valid, writeable file
    FILE* newIndex = fopen(newIndexFilename, "w+");
    if(newIndex == NULL){
        fprintf(stderr, "Error: newIndexFilename is not a valid writeable file path.");
        exit(3);
    }

    //save the new index 
    index_save(index, newIndexFilename);
    //delete all indices 
    index_delete(index);
    //close files
    fclose(oldIndex);
    fclose(newIndex);
}

