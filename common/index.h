/* 
 * index.c
 *
 * An inverted-index data structure.
 * Functions to load, save, delete indexes as well. 
 *
 * Paul Chirkov October 2023
 */

#include <stdio.h>
#include <counters.h>

/**************** global types ****************/
typedef struct index index_t;

/**************** functions ****************/

/**************** index_new ****************/
 /* Create a new inverted index.
 *
 * We return:
 *   pointer to a new index, or NULL if error.
 * We guarantee:
 *   The index is initialized empty.
 * Caller is responsible for:
 *   later calling index_delete.
 */
index_t* index_new(int size);

/**************** index_add ****************/
/* Increment counter for word with certain countID
 * 
 * Caller provides:
 *   valid pointer to index, a key (the word), and a valid document ID.
 * We return:
 *  true if succesfully incremented.
 *  false otherwise.
 * We guarantee:
 *   the incremented counter's value will be >= 1, on successful return.
 * We do:
 *  ignore if index or key is NULL.
 *  if the key's counter does not yet exist, counter is created for it and 1 is addedd.
 *  if the key does exist, increment its counter by 1.
 */
bool index_add(index_t* index, char* word, int docID);

/**************** index_set ****************/
/* Set counter for word with certain countID
 * 
 * Caller provides:
 *   valid pointer to index, a key (the word), and a valid document ID.
 * We return:
 *  true if succesfully set.
 *  false otherwise.
 * We do:
 *  ignore if index or key is NULL.
 *  if the key's counter does not yet exist, counter is created for it.
 */
bool index_set(index_t* index, char* word, int docID, int count);

/**************** index_delete ****************/
/* Delete the whole index.
 *
 * Caller provides:
 *   a valid pointer to an index.
 * We do:
 *   we ignore NULL ctrs.
 *   we free all memory we allocate for this index.
 */
void index_delete(index_t* index);

/**************** index_save ****************/
/* Write index data structure to a file. 
 *
 * Caller provides:
 *   a valid pointer to an index, a desired pathname for the file.
 * We do:
 *   we ignore NULL index or invalid pathname.
 *   overwrite file if it all ready exists.
 *   output inverted index to the file.
 */
void index_save(index_t* index, char* pathName);

/**************** index_load ****************/
/* Write index data structure to a file. 
 *
 * Caller provides:
 *   a valid pointer to an index, a desired pathname for the file.
 * We return:
 *  the newly created index data struct. 
 * We do:
 *  load inverted index data structure from an index file.
 * Caller is responsible for:
 *  later calling index_delete() on the index.
 */
index_t* index_load(char* pathName);


counters_t* index_getCounters(index_t* index, char* word);