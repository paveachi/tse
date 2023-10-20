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
#include <file.h>

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

//makes sure that a directory is in fact crawler produced.
bool pagedir_validate(char* pageDirectory){
    //if fopen pathname.crawler then it is a valid and return true
    char* pathName = (char*)malloc(strlen(pageDirectory) + 10); // 10 for "/.crawler" and null terminator
    if (pathName == NULL) {
        return false;  // memory allocation failed
    }

    sprintf(pathName, "%s/.crawler", pageDirectory);

    FILE* file = fopen(pathName, "r");

    if(file == NULL){
        free(pathName);
        return false;
    }
    free(pathName);
    return true;
}

// to load a page from a file in directory... create a webpage from a file??
//WEBPAGE RETURNED MUST LATER BE DELETED
webpage_t* pagedir_load(char* pageDirectory, int docID){
    //return webpage_t... created by reading the file at doc ID
    //construct path
    char* pathName = (char*)malloc(strlen(pageDirectory) +10); // 9 digit max ID. 
    if (pathName == NULL) {
        return NULL;  // Memory allocation failed
    }
    sprintf(pathName, "%s/%d", pageDirectory, docID);
    //open file from pathname
    FILE* file = fopen(pathName, "r");
    if(file == NULL){
        free(pathName);
        return NULL; //error opening file (doesn't exist)
    }

    //read first line of file into new webpage object
    char* url = file_readLine(file); //MUST FREE THIS SHIT

    int depth; 
    fscanf(file, "%d", &depth);
    
    webpage_t* page = webpage_new(url, depth, NULL);

    free(pathName); //DO I FREE URL MYSELF?

    return page;

}

