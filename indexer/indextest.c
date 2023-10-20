#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <index.h>
#include <pagedir.h>


int main(const int argc, char* argv[]){
    
//not particularly difficult
//validate arguments
const char* progName = argv[0];

if(argc != 3){
        fprintf(stderr, "Usage: %s oldIndexFilename newIndexFilename.\n", progName);
        exit(1);
}

char* oldIndexFilename = argv[1];
char* newIndexFilename = argv[2];

FILE* oldIndex = fopen(oldIndexFilename, "r");
if(oldIndex == NULL){
    fprintf(stderr, "Error: oldIndexfile does not exists or is not readable.\n");
    exit(2);
}
//index_load a path into an index struct....
index_t* index = index_load(oldIndexFilename);

FILE* newIndex = fopen(newIndexFilename, "w+");
if(newIndex == NULL){
    fprintf(stderr, "Error: newIndexFilename is not a valid writeable file path.");
    exit(3);
}

//run index_save on it
index_save(index, newIndexFilename);
fclose(oldIndex);
fclose(newIndex);
}

