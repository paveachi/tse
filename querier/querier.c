#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include <pagedir.h>
#include <webpage.h>
#include <index.h>
#include <word.h>
#include <counters.h>
#include <file.h>

void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
void parseQuery(/*maybe char query*/);
void tokenize(char* query, char** wordArray, int queryLen);
void countersUnion(counters_t* ctrs1, counters_t* ctrs2);
void countersUnionHelper(void* ctrs1, const int key, const int count2);
void countersIntersection(counters_t* ctrs1, counters_t* ctrs2);
void countersSameKeys(void* ctrs2, const int key, const int count1);
void countersIntersectionHelper(void* ctrs1, const int key, const int count1);
void rankScores(counters_t* ctrs);
void rankHelper(void* arg, const int key, const int count);
void calculateScores(char** query, char* indexPath, int queryLen);
int numWords(char* query);
void countMatches(void* arg, const int key, const int count);
 

struct max{
        int maxKey;
        int maxCount;
};

//basic idea is that querier is broken up into three main functions
int main(const int argc, char* argv[]){
    char* pageDirectory;
    char* indexFilename;
    
    //call parseargs()
    parseArgs(argc, argv, &pageDirectory, &indexFilename);
    
    //READ ONE LINE AT A TIME DON't ADD NEWLINE TO CHARACTER
    
    while(!feof(stdin)){
        printf("query?\n");
        char* query = file_readLine(stdin);
        //loop through query character by character when query[i] is a space, increment word count!
        if(feof(stdin)){
            exit(0);
        }
        parseQuery(query, indexFilename);
        free(query);
    }
    // exit(0);
}

//parseArgs... same as others validate command line 
void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename){
    //check that argv[1] is a crawler directory
    //extract into variable
    if(argc != 3){
        fprintf(stderr, "must have 3 arguments\n");
        exit(1);
    }

    if(!pagedir_validate(argv[1])){
        fprintf(stderr, "The given pageDirectory is not valid. It needs to be a directory with a .crawler file.\n");
        exit(2);
    }
    *pageDirectory = argv[1];

    // check that argv[2] is a file that can be read
    FILE* file = fopen(argv[2], "r");
    if(file == NULL){
        fprintf(stderr, "Not a valid path for the index file.\n");
        exit(3);
    }

    *indexFilename = argv[2];
    fclose(file);
    //extract into variable
}



//parse querry... get query, check exit non zero if invalid...
// variable to keep track of previous conjuction (starts at true). 
// bool prev conjuction. If second to last also true;
// create array of words in query
void parseQuery(char* query, char*indexFilename){

    
    //read query from command line into a string. Each time you read call all this shit.
    
    if(query == NULL || strlen(query) == 0){
        fprintf(stderr, "error, there is no query.\n");
        exit(2);
    }

    //verify only contains letters and spaces
    for(int i=0; query[i]; i++){
        if(!(isalpha(query[i]) || isspace(query[i]))){
            fprintf(stderr, "error, must only contain letters and spaces.\n");
            exit(2);
        }
    }

    int queryLen = numWords(query);
    char** wordArray = calloc(queryLen, sizeof(char*));
    tokenize(query, wordArray, queryLen);
    //char** wordArray = calloc(arrLength, sizeof(char*));
    //FREED EVENTUALLY MUST BE!!!
    
    

    //print clean query AND make it all lower case IMPORTANT. Could Make this shit a function...
    printf( "The clean query: ");
    for(int i =0; i<queryLen; i++){
        if(wordArray[i] == NULL){
            break;
        }
        else{
            word_normalize(wordArray[i]);
            printf( "%s ", wordArray[i]);
        }
    }
    printf("\n");

    
    //I COULD MAKE THIS A SEPERATE FUNCTION RETURN TRUE/FALSE
    //now validate structure of query... loop through array. 
        //make sure doesn't start with operator
    if((strcmp(wordArray[0], "or") ==0) || (strcmp(wordArray[0], "and")==0)){
        fprintf(stderr, "error, query cannot start with 'and' or 'or\n");
        exit(3);
    }

    //THIS DOESN'T WORK
    //doesn't end with operator
    if((strcmp(wordArray[queryLen - 1], "or")==0) || (strcmp(wordArray[queryLen - 1], "and")==0)){
        fprintf(stderr, "error, query cannot end with 'and' or 'or\n");
        exit(3);
    }
    //no operators are adjacent
    bool prev = false;
    for(int i =0; i<queryLen; i++){
        if((strcmp(wordArray[i], "or")==0 )|| (strcmp(wordArray[i], "and")==0)){
            if(prev){
                fprintf(stderr, "error, two operators cannot be next to each other\n");
                exit(4);
            }
            else{
                prev = true;
            }
        }
        else{
            prev = false;
        }
    }
    
    //could call calcscores here... can free 
    calculateScores(wordArray, indexFilename, queryLen);
    free(wordArray);
}

//THIS SHIT IS NOT FUNCTIONING PROPERLY. WRITE FUNCTION (like words) to get num words in query... 
void tokenize(char* query, char** wordArray, int queryLen){

    char* word = query;

    //I THINK PROBLEM IS NOT KNOWING WHEN TO STOP (MAXLEN). 
    for(int i = 0; i < queryLen; i++){

        while((!isalpha(*word)) && (*word!='\0')){
            word++;
        }
        if(*word == '\0'){
            break;
        }
        if(isalpha(*word)){
            wordArray[i] = word;
            // word = char
            // char* rest = strchr(word," ")
            char* rest = strchr(word, ' ');
            if(rest == NULL){
                break;
            }
            // set rest to be a null terminator..
            *rest = '\0';
            //move past null terminator.. SHOULD it be rest + 1?????
            word = rest+1;
        }
        
    }
}


//loop through query word array and calculate the scores. This is the beast algo
//cereates 
//return a counters_t object w/ the scores... feed counters_t object into sort scores. 
//delete that shit.

void calculateScores(char** query, char* indexPath, int queryLen){
    //have to loadIndex from path...
    index_t* index = index_load(indexPath);

    //prevWord = and.... initialize
    char* prev = "and";
    //loop through the array of queries, calculate the scores
    //initialize runningSum = 0 and runningProd = first counters. both empty counters?
    //set first word counters to runningProd.. create new runningProd
    //MUST BE FREEEEED!!!!!
    counters_t* runningSum = counters_new();
    //set running prod to counter object of first word
    counters_t* runningProd;
    //loop through the array of queries, calculate the scores
    for(int i =0; i < queryLen; i++){
        //if word is 'and'
        if(strcmp(query[i], "and") == 0){
            prev = query[i];
            continue;
        }
        if(strcmp(query[i], "or") == 0){
            prev = query[i];
            continue;
        }
        //WHAT IF THIS IS NULL OR DOESN'T EXIT
        counters_t* current = index_getCounters(index, query[i]);
        //IF CURRENT == NULL {continue??}
        if(strcmp(prev, "or") == 0){
            // RunningSum U runningProd
            countersUnion(runningSum, runningProd);
            // set runningProd to new set at word
            runningProd = current; 
        }
        else{
            // take the intersetion of runingProd and set at WORD
            if(i == 0){
                runningProd = current;
            }
            //take intersection of runningProd and set at word
            if(current!= NULL){
                countersIntersection(runningProd, current);
            }
            else{
                runningProd = NULL;
            }
        }
        prev = query[i];
    }
    //need to add the last running prod back to sum no matter waht.
    countersUnion(runningSum, runningProd); 

    /*
    printf("\nThis is the final return value for the counters: \n");
    counters_print(runningSum, stdout);
    printf("\n is it correct?");
    */
    //call RANK SCORES
    //iterate through running sum, if count!= zero matches++. 
    int matches=0;
    counters_iterate(runningSum, &matches, countMatches);
    printf("Matches %d documents (ranked):\n", matches);

    rankScores(runningSum);

    //delete INDEX, running prod, running sum
    index_delete(index);
    counters_delete(runningSum);
}

void countMatches(void* arg, const int key, const int count){
    int* matches = arg;
    if(count != 0){
        (*matches)++;
    }
}

//sortScores(counters_t)
//take in counters object. Iterate and remove largest. set to zero. 
//repeat until none left/
void rankScores(counters_t* ctrs){
    //just needs to loop number of times or until zero is found...
    //initialize "max struct"...
    // include STRING URL IN MAX STRUCT...
    if(ctrs != NULL){
    struct max maxCtr = {0,-1};

    while(maxCtr.maxCount != 0){
        maxCtr.maxCount = -1;
        maxCtr.maxKey = 0;
        counters_iterate(ctrs, &maxCtr, rankHelper);
        if(maxCtr.maxCount <= 0){
            break;
        }
        //MAKE STRING FORMATTING NICE HERE.. PRINT//TOGET URL I COULD LOAD WEBPAGE AND THEN DELETE IT,
        printf("Score: %d   doc  %d\n", maxCtr.maxCount, maxCtr.maxKey);
        counters_set(ctrs, maxCtr.maxKey, 0);
    }
    }

    //iterate through set and remove largerest
}

void rankHelper(void* arg, const int key, const int count){
    struct max* max = arg;
    if(count > max->maxCount){
        max->maxKey = key;
        max->maxCount = count;
    }

}


void countersUnion(counters_t* ctrs1, counters_t* ctrs2){
    counters_iterate(ctrs2, ctrs1, countersUnionHelper);
}

// helper function, intersection of two countersets (and)

void countersUnionHelper(void* ctrs1, const int key, const int count2){
    int count1;
    if((count1 = counters_get(ctrs1, key)) != 0){
        counters_set(ctrs1, key, count2 + count1);
    }
    else{
        counters_set(ctrs1, key, count2);
    }
}

void countersIntersection(counters_t* ctrs1, counters_t* ctrs2){
    // loop through the ctrs1 and check
    counters_iterate(ctrs1, ctrs2, countersSameKeys);
    //loop again and make sure if doesn't exist in second one set to zero. 
    counters_iterate(ctrs2, ctrs1, countersIntersectionHelper);
}

void countersSameKeys(void* ctrs2, const int key, const int count1){
    //loop through 1 if key not in 2 set key to 0 in 2. This makes sure we don't miss any when intersecting the sets...
    if(counters_get(ctrs2, key) == 0){
        counters_set(ctrs2, key, 0);
    }
}

void countersIntersectionHelper(void* ctrs1, const int key, const int count1){
    //loop through 2 and set count of each key in 1 to min between count, key.
    int count2;
    if((count2 = counters_get(ctrs1, key)) != 0){
        if(count1 < count2){
            counters_set(ctrs1, key, count1);
        }
    }

}

int numWords(char* query){
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
    //if(isalpha(query[strlen(query) -1])){
    //    queryLen++;
   // }
    return queryLen;
}