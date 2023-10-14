/* 
 * pagedir.c - CS50 'pagedir' module
 *
 * see pagedir.h for more information.
 *
 * Paul Chirkov October 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <webpage.h>
#include <stdbool.h>
#include <string.h>

bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);


/**************** pagedir_init ****************/
/* see pagedir.h for description */
bool pagedir_init(const char* pageDirectory){
    if(pageDirectory == NULL){
        return false;
    }

    // construct the pathname for the .crawler file in that directory
    char* pathName = (char*)malloc(strlen(pageDirectory) + 10); // 10 for "/.crawler" and null terminator
    if (pathName == NULL) {
        return false;  // memory allocation failed
    }

    sprintf(pathName, "%s/.crawler", pageDirectory);
    // open the file for writing; on error, return false.
    FILE* file = fopen(pathName, "w+");
    if(file == NULL){
        free(pathName);
        return false;
    }
    // close the file and return true.
    fclose(file);
    free(pathName);
    return true;
}

/**************** pagedir_save ****************/
/* see pagedir.h for description */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID){
    //make sure inputs are valid
    if(page == NULL || pageDirectory == NULL || docID< 0){
        return;
    }
    //construct the pathname for the page file in pageDirectory
    char* pathName = (char*)malloc(strlen(pageDirectory) +10); // 9 digit max ID. 
    if (pathName == NULL) {
        return;  // Memory allocation failed
    }
    sprintf(pathName, "%s/%d", pageDirectory, docID);
     
    //open file for writing
    FILE* file = fopen(pathName, "w+");
    if(file == NULL){
        free(pathName);
        return;
    }
    //print the URL
    fprintf(file, "%s\n", webpage_getURL(page));
    //print the depth
    fprintf(file, "%d\n", webpage_getDepth(page));
    //print the contents of the webpage
    fprintf(file, "%s\n", webpage_getHTML(page));
    //close the file
    fclose(file);
    free(pathName);
}