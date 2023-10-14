
/* crawler.C 
 * 
 * A program for crawling a webpage and gathering all the urls on it. 
 *
 * Paul Chirkov October 2023
 */

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
/* Handles command line arguments, makes sure they are all valid. 
* 
* Caller provides:
*   num arguments, array of arguments, pointers to variables that should be updated by arguments
* We guarantee: 
*   If the function doesn't exit non-zero, the arguments are valid
* We return:
*   only return when succesful, otherwise exit non-zero
*/
static void parseArgs(const int argc, char* argv[],char** seedURL, char** pageDirectory, int* maxDepth){
    
    const char* progName = argv[0]; // name of this program
    // make sure there is a correct number of arguments
    if (argc != 4) {
        fprintf(stderr, "usage: %s seedURL pageDirectory maxDepth\n", progName);
        exit(1);
    }

    // normalize the url and make sure it is internal
    char* URLcheck = normalizeURL(argv[1]);
    if(!isInternalURL(URLcheck)){
        fprintf(stderr, "Must use an internal URL.\n");
        exit(2);
    }
    *seedURL = URLcheck;

    // make sure that pageDirectory is a valid path
    if(!pagedir_init(argv[2])){
        fprintf(stderr, "the specified directory does not exist.\n");
        exit(3);
    }
    *pageDirectory = argv[2];
    
    // check third argument is a number.
    int num = -1;
    if(sscanf(argv[3], "%d", &num) != 1){
       fprintf(stderr, "Max depth must be a number in the range [0,10].\n");
        exit(4); 
    }
    // check that the number is in valid range
    if(!(num >= 0 && num <=10)){
        fprintf(stderr, "Max depth must be in the range [0,10].\n");
        exit(5);
    }
    
    *maxDepth = num;    
}

/**************** crawl ****************/
/* Crawls the webpage at the seed URL and all embedded URLS to given depth, saves page to pageDirectory.  
*
* Caller Provides:
*   starting URL, existing directory to store pages and the max search depth.
* We do: 
*   Extract webpage from bag. Fetch HTML and save it if succesful.
*   Add all urls in page to bag if valid depth and then repeat. 
* Notes:
*   The memory allocated to new webpages, the bag and the hashtable are all freed by their respective delete() functions.
*/
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth){

    //initialize the hashtable and add the seedURL
    hashtable_t* hashtable = hashtable_new(200);
    hashtable_insert(hashtable, seedURL, "");
    //initialize the bag 
    bag_t* bag = bag_new();
    //create and add webpage representing the seedURL at depth 0 
    webpage_t* page = webpage_new(seedURL, 0, NULL);
    bag_insert(bag, page);

    int docID = 1;
    webpage_t* current;
    //while bag is not empty 
    while((current = bag_extract(bag)) != NULL){
        //pull a webpage from the bag 
        //fetch the HTML for that webpage and if fetch was successful
        if(webpage_fetch(current)){
            printf("%d %9s: %s\n", webpage_getDepth(current), "Fetched", webpage_getURL(current));
            //save the webpage to pageDirectory
            pagedir_save(current, pageDirectory, docID++);
            //if the webpage is not at maxDepth,
            if(webpage_getDepth(current) < maxDepth){
                //pageScan that HTML
                printf("%d %9s: %s\n", webpage_getDepth(current), "Scanning", webpage_getURL(current));
                pageScan(current, bag, hashtable);
            }
        }
        //delete webpage, free memory
        webpage_delete(current);
    }
    //delete hashtable
    hashtable_delete(hashtable, NULL);
    //delete bag
    bag_delete(bag, NULL);
}

/**************** pageScan ****************/
/* Function for adding all embedded urls to the bag if they are valid.
*
* Caller provides:
*   Page to scan, bag to add new pages, hashtable of URLS already seen
* We guarantee: 
*   If URL is not internal or already in the hashtable, the memory that holds it is freed. 
*/
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen){
    
    int pos = 0;
    char* result;
    //while there is another URL in the page
    while((result = webpage_getNextURL(page, &pos)) != NULL){
        //normalize URL
        char* normalizedResult = normalizeURL(result);
        free(result);
        //check if it is internal
        if(isInternalURL(normalizedResult)){
            //insert the webpage into the hashtable
            if(hashtable_insert(pagesSeen, normalizedResult, "")){
                //create a webpage_t for it.
                webpage_t* newPage = webpage_new(normalizedResult, webpage_getDepth(page) + 1, NULL);
                printf("%d %9s: %s\n", webpage_getDepth(page), "Found", webpage_getURL(newPage));
                //insert the webpage into the bag
                bag_insert(pagesToCrawl, newPage);
                printf("%d %9s: %s\n", webpage_getDepth(page), "Added", webpage_getURL(newPage));
            }
            else{
                printf("%d %9s: %s\n", webpage_getDepth(page), "IgnDupl", normalizedResult);
                free(normalizedResult); // free URL if it fails to be inserted into hashtable
            } 
        }
        else{
            printf("%d %9s: %s\n", webpage_getDepth(page), "IgnExtrn", normalizedResult);
            free(normalizedResult); // free if it's not internal.
        }
    }
}


