//Function for normalizing words

#include <ctype.h>


void word_normalize(char* word){
    for(int i=0; word[i]; i++){
        word[i] = tolower(word[i]);
    }
}