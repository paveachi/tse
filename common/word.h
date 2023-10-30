
#include <ctype.h>
#include <stdbool.h>

/**************** word_normalize ****************/
/* Normalizes a word.
* We return:
*   Nothing
* Caller provides:
*   Word to normalize
*/
void word_normalize(char* word);


/**************** word_numWords ****************/
/* Counts number of words in a string.
* Caller provides:
*   the string. 
* We return:
*   The integer number of words in the query.
* Assumes: 
*   The query is only letters and spaces.
*/
int word_numWords(char* query);
