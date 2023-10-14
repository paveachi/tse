
#include <stdio.h>
#include <stdlib.h>
#include <webpage.h>
#include <stdbool.h>
#include <pagedir.h>
#include <ctype.h>
#include <bag.h>
#include <hashtable.h>

static void parseArgs(const int argc, char* argv[],char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);


/**************** main ****************/
int main(const int argc, char* argv[]){
    char* seedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth =0;

    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);

    exit(0);
}


/**************** parseArgs ****************/
/*
*
* We guarantee: 
*   The arguments are valid if doesn't exit
*/
static void parseArgs(const int argc, char* argv[],char** seedURL, char** pageDirectory, int* maxDepth){
    const char* progName = argv[0]; // name of this program

    //make sure there is a correct number of arguments
    if (argc != 4) {
        fprintf(stderr, "usage: %s seedURL pageDirectory maxDepth\n", progName);
        exit(1);
    }

    //normalize the url and make sure it is internal
    char* URLcheck = normalizeURL(argv[1]);

    if(!isInternalURL(URLcheck)){
        fprintf(stderr, "Must use an internal URL.\n");
        exit(2);
    }
    *seedURL = URLcheck;

    //make sure that pageDirectory is a valid path
    if(!pagedir_init(argv[2])){
        fprintf(stderr, "the specified directory does not exist.\n");
        exit(3);
    }
    *pageDirectory = argv[2];
    
    //CALL MAX NUM. CHECK THAT THIS SHIT IS A NUMBER AND NOT SOME STRING!!!!!!!
    //check that maxDepth is a number and greater than 0.

    int num = atoi(argv[3]);
    if(!(num >= 0 && num <=10)){
        fprintf(stderr, "Max depth must be in the range [0,10].\n");
        exit(4);
    }
    
    //maybe needs to be dereferenced?
    *maxDepth = num;    
}

/**************** crawl ****************/
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth){

    //initialize the hashtable and add the seedURL
    hashtable_t* hashtable = hashtable_new(200);
    hashtable_insert(hashtable, seedURL, "");
    //initialize the bag 
    bag_t* bag = bag_new();
    //add a webpage representing the seedURL at depth 0 
    webpage_t* page = webpage_new(seedURL, 0, NULL);
    bag_insert(bag, page);

    int docID = 1;
    webpage_t* current;
    //while bag is not empty 
    while((current = bag_extract(bag)) != NULL){
    //  pull a webpage from the bag 
    //	fetch the HTML for that webpage
        //	if fetch was successful,
        if(webpage_fetch(current)){
    //		save the webpage to pageDirectory
            pagedir_save(current, pageDirectory, docID++);
    //		if the webpage is not at maxDepth,
            if(webpage_getDepth(current) < maxDepth){
    //			pageScan that HTML
                pageScan(current, bag, hashtable);
            }
        }
    //delete that webpage.. Don't want to delete if it is already freed. 
        webpage_delete(current);
    }
    //delete the hashtable
    hashtable_delete(hashtable, NULL);
    //delete the bag
    bag_delete(bag, NULL);
}

/**************** pageScan ****************/
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen){
    
    int pos = 0;
    char* result;
    //while there is another URL in the page
    while((result = webpage_getNextURL(page, &pos)) != NULL){
    //	normalize URL
        char* normalizedResult = normalizeURL(result);
        free(result);
        //check if it is internal
        if(isInternalURL(normalizedResult)){
    //		insert the webpage into the hashtable
            //do I INSSERT THE RESULT INTO HASHTABLE???????
            if(hashtable_insert(pagesSeen, normalizedResult, "")){
    //			create a webpage_t for it.
                webpage_t* newPage = webpage_new(normalizedResult, webpage_getDepth(page) + 1, NULL);
    //			insert the webpage into the bag
                bag_insert(pagesToCrawl, newPage);
            }
            else{
                free(normalizedResult); // free URL only if it fails to be inserted into hashtable
            } 
        }
        else{
            free(normalizedResult);
        }
    }
}


