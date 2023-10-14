
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
 * Caller provides:
 *   valid web page, valid directory, ID number of webpage.
 * We guarantee:
 *  webpage is saved or nothing happens if there is an error.
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);