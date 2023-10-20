#include <stdio.h>

typedef struct index index_t;


index_t* index_new(int size);


bool index_add(index_t* index, char* word, int docID);

bool index_set(index_t* index, char* word, int docID, int count);


void index_delete(index_t* index);

void index_save(index_t* index, char* pathName);

index_t* index_load(char* pathName);