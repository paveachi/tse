
/* 
 * pagedir.h
 * 
 * A collection of functions for initializing a crawler directory and saving the webpages to it.
 *
 * Paul Chirkov October 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <webpage.h>
#include <stdbool.h>

/**************** functions ****************/

/**************** pagedir_init ****************/
/* initialize the directory to store the webpages
 *
 * Caller provides:
 *   valid path of previously existing directory.
 * We return:
 *   return TRUE if intilization is succesful and .crawler file created FALSE otherwise.
 */
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/* Create a file for webpage, save it to directory. Write url, number and html to file.
 *
 * Caller provides:
 *   valid web page, valid directory, ID number of webpage.
 * We guarantee:
 *  webpage is saved or nothing happens if there is an error.
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_validate ****************/
/* validate that given directory was procuced by the crawler.
 *
 * Caller provides:
 *   path to directory
 * We return:
 *   true if .crawler file exists in directory.
 *   false otherwise.
 */
bool pagedir_validate(char* pageDirectory);

/**************** pagedir_load ****************/
/* Loads webpage from crawler created webpage file.
 *
 * Caller provides:
 *  The pathname to the .crawler directory and the ID of the webpage.
 * We return:
 *  The webpage struct created from the URL in the given file. 
 *  NULL if error.
 * Note:
 *  The HTML is not read into the new webpage object and must be fetched() if it is desired.
 */
webpage_t* pagedir_load(char* pageDirectory, int docID);