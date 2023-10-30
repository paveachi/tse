/* 
 * word.c - CS50 'word' module.
 *
 * see word.h for more information.
 *
 * Paul Chirkov October 2023
 */

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

/**************** word_normalize ****************/
/*see word.h for more details*/
void word_normalize(char* word){
    for(int i=0; word[i]; i++){
        word[i] = tolower(word[i]);
    }
}

/**************** word_numWords ****************/
/*see word.h for more details*/
int word_numWords(char* query){
    bool prevSpace = 1;
    int queryLen = 0;
    for(int i = 0; i < strlen(query); i++){
        if(isalpha(query[i]) && prevSpace){
            queryLen++;
            prevSpace = 0;
        }
        else if(isspace(query[i])){
            prevSpace = 1;
        }
    }
    return queryLen;
}