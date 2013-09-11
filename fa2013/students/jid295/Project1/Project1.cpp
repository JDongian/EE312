/*
 * Project1.c
 * Please insert the standard EE312 header here!
 * 
 * EE312 Project 1 (Spell Check)
 * Your Name
 * Your TA's Name
 * Your Recitation Time
 */

#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE (1 << 20)
#define BUFFER_SIZE2 (1 << 24)

char* indexedDict[BUFFER_SIZE2] = {NULL}; // a large buffer to hold index of dictionary

bool isLetter(char letter) {
    return (('a' <= letter && letter <= 'z') ||
            ('A' <= letter && letter <= 'Z'));
}

/* Lowercase-ify the given string. */
char* toLower(char word[]) {
    int i = 0;
    //printf("\nLowercasing...\n");
    while(word[i]) {
        if('A' <= word[i] && word[i] <= 'Z')
            word[i] -= 'A'-'a'; //Offset the word to make it lowercase.
        i++;
    }
    return word;
}

char* strcpy(char *target, const char *source) {
    //printf("\ncopying '");
    //printf(source);
    //printf("' -> '");
    //printf(target);
    //printf("'.");
    int i;
    if(!source) {
        target = NULL;
        return NULL;
    }
    for(i = 0; source[i] != '\0'; ++i)
        target[i] = source[i];
    target[i] = source[i];
    return target;
}

/* Case-insensitive string comparison */
/*
bool compareStrings(char str1[], char str2[]) {
    //printf("\ncomparing...");
    toLower(str1);
    toLower(str2);
    //printf("\ncomparing '");
    //printf(str1);
    //printf("' and '");
    //printf(str2);
    //printf("'.");
    for(;*str1 == *str2; str1++, str2++) {
        if(*str1 == '\0'){
            return true;
        }
    }
    return false;
}*/

/* A simple string comparison
 * str1 < str2 returns -1
 * str1 = str2 returns 0
 * str1 > str2 returns 1
 * NOTE: considers \n to be the end of string as well as \0
 */
int compare(char* str1, char* str2) {
    ////toLower(str1);
    ////toLower(str2);
    //printf("\ncomparing '");
    //printf(str1);
    //printf("' and '");
    //printf(str2);
    //printf("'.");
/*    while(1) {
        if(*str1 == *str2 ||
           ((!*str1 || *str1 == '\n') && (!*str2 || *str2 == '\n'))) {
            if(*str1 && *str1 != '\n') {
                ++str1;
                ++str2;
                continue;
            } else {
                return 0;
            }
        }
        if(*str1 < *str2) {
            return -1;
        } else if(*str1 > *str2) {
            return 1;
        }
    }*/
    while(1) {
        if(*str1 == *str2) {
            if(*str1 > '\n') {
            //if(*str1 && *str1 != '\n') {
                ++str1;
                ++str2;
                continue;
            } else {
                return 0;
            }
        }
        if((!*str1 || *str1 == '\n') && (!*str2 || *str2 == '\n'))
            return 0;
        if(*str1 < *str2)
            return -1;
        if(*str1 > *str2)
            return 1;
    }
}

/* Return the next word in the given article,
 * moving the pointer of the article to the next word.
 * Basically, it's like 'pop the next word' except on a string. 
 */

char* iterateByWord(char* article[]) {
    char nextWord[BUFFER_SIZE] = {'\0'}; // a large buffer to hold a word(1MB)
    int i = 0;
    //printf("\nitr: ");
    while(**article && !isLetter(**article)) { ++(*article); }
    while(**article && isLetter(**article)) { nextWord[i++] = *(*article)++; }
    while(**article && !isLetter(*((*article)++))) {}
    if(nextWord[0]) {
        if((*article)[1]) {
            --(*article);
        }
        //printf(nextWord);
        return nextWord;
    } else {
        //printf("NULL");
        return NULL;
    }
}

//bool isInDictionary(char word[], char dictionary[]) {
//    while(1) {
//        char nextWord[BUFFER_SIZE] = {'\0'}; // a large buffer to hold a word(1MB)
//        strcpy(nextWord, iterateByWord(&dictionary));
//        //printf("\ndict test on '");
//        //printf(word);
//        //printf("': ");
//        //printf(nextWord);
//        if(nextWord[0]) {
//            toLower(nextWord);
//            if(compareStrings(nextWord, word)) {
//                return true;
//            }
//        } else {
//            return false;
//        }
//        /*if(!dictionary[0]) {
//            //printf("\ndict finished");
//            return false;
//        } else {
//            toLower(nextWord);
//            if(compareStrings(nextWord, word)) {
//                return true;
//            }
//        }*/
//    }
//}

void indexDictionary(char* dictionary) {
    int i = 0;
    printf("\nIndexing...");
    while(*dictionary) {
        while(*dictionary && !isLetter(*dictionary)) { ++dictionary; }
        if(isLetter(*dictionary)) {
            indexedDict[i++] = dictionary;
        } else {
            break;
        }
        while(isLetter(*dictionary)) { ++dictionary; }
        while(*dictionary && !isLetter(*dictionary)) { ++dictionary; }
    }
    printf("\nDone indexing.");
}

void printIndexedDict(char* indexedDictionary[BUFFER_SIZE2]) {
    int i = 0;
    int j = 0;
    while(indexedDictionary[i][0]) {
        j = 0;
        putchar('"');
        while(indexedDictionary[i][j] != '\n' &&
              indexedDictionary[i][j]) {
            putchar(indexedDictionary[i][j++]);
        }
        putchar('"');
        putchar('\n');
        /* Exit if the next pointer is NULL. */
        if(!indexedDictionary[++i]) {
            break;
        }
    }
}

int indexedDictLen(char* indexedDictionary[BUFFER_SIZE2]) {
    int i = 0;
    while(indexedDictionary[i]) { ++i; }
    return i;
}

/* Selection sort the given array of strings. */
void sortIndexedDict(char* indexedDictionary[BUFFER_SIZE2]) {
    int i, j;
    int iMin;
    int length = indexedDictLen(indexedDictionary);
    char* tmpWord;
     
    printf("\nSorting...\n");
    
    for(j = 0; j < length-1; j++) {
        iMin = j;
        //printf("\nNEXT %d\n", j);
        for(i = j+1; i < length; i++) {
            //printf("\nCMP %d\n", i);
            if(compare(indexedDictionary[i], indexedDictionary[iMin]) == -1) {
                iMin = i;
            }
        }
        /* iMin is index of minimum element.
         * Swap it with current position j.
         */
        if(iMin != j) {
            //printf("\nSWAP\n");
            tmpWord = indexedDictionary[j];
            indexedDictionary[j] = indexedDictionary[iMin];
            indexedDictionary[iMin] = tmpWord;
        }
    }
    printf("Done sorting.");
}

bool inDict(char* key, char* indexedDictionary[BUFFER_SIZE2]) {
    int imin = 0;    
    int imax = indexedDictLen(indexedDictionary);
    int imid = 0;
    toLower(key);
    //printIndexedDict(indexedDictionary);
    while(imin < imax) {
        imid = (imin+imax)/2;
        //printf("\nmin:%d mid:%d max:%d", imin, imid, imax);
        // Guarantee interval reduced each iteration
        if(!(imid < imax)) {
            printf("ERROR");
        }
        if(compare(indexedDictionary[imid], key) == -1) {
            imin = imid+1;
        } else {
            imax = imid;
        }
    }
    /* Key is now at imin or not in dict. */
    return((imax == imin) && (compare(indexedDictionary[imin], key) == 0));
}

/*
   void spellCheck(char article[], char dictionary[]) {
    toLower(dictionary);
    indexDictionary(dictionary);
    //sortIndexedDict(indexedDict);
    while(1) {
        char nextWord[BUFFER_SIZE] = {'\0'}; // a large buffer to hold a word(1MB)
        strcpy(nextWord, iterateByWord(&article));
        //printf("\nlooping on '");
        //printf(article);
        //printf("': '");
        //printf(nextWord);
        //printf("'.");
        if(nextWord[1] && !inDict(nextWord, indexedDict)) { //Take care of 1 letter words.
            printf("\n");//RESULT: ");
            printf(nextWord);
        }
        if(!article[1]) {
            //printf("\nDONE.");
            return;
        }
    }
}*/
void spellCheck(char article[], char dictionary[]) {
    int i;
    char nextWord[BUFFER_SIZE]; // a large buffer to hold a word(1MB)
    toLower(dictionary);
    indexDictionary(dictionary);
    //sortIndexedDict(indexedDict);
    while(1) {
        i = 0;
        while(nextWord[i])
            nextWord[i++] = '\0';
        strcpy(nextWord, iterateByWord(&article));
        //printf("\nlooping on '");
        //printf(article);
        //printf("': '");
        //printf(nextWord);
        //printf("'.");
        if(nextWord[1] && !inDict(nextWord, indexedDict)) { //Take care of 1 letter words.
            printf(nextWord);
            printf("\n");
        }
        if(!article[1]) {
            return;
        }
    }
}
