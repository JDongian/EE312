/*
 * Project1.c
 * 
 * EE312 Project 1 (Spell Check)
 * Joshua Dong
 * Steven
 * 3-4 Friday
 */

#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE (1 << 20)
#define BUFFER_SIZE2 (1 << 24)

/* A large buffer to hold dictionary indexes*/
char* indexedDict[BUFFER_SIZE2] = {NULL}; 

/* Verifies that the given char is in the English alphabet. */
bool isLetter(char letter) {
    /* Short circuit for lowercase ;) */
    return (('a' <= letter && letter <= 'z') ||
            ('A' <= letter && letter <= 'Z'));
}

/* Lowercase-ify the given string. */
char* toLower(char word[]) {
    int i = 0;
    //printf("\nLowercasing...\n");
    while(word[i]) {
        if('A' <= word[i] && word[i] <= 'Z') {
            /* Offset word to lowercase. */
            word[i] -= 'A'-'a'; 
        }
        i++;
    }
    return word;
}

/* A simple string comparison:
 *     Cases:
 *         str1 < str2 returns -1
 *         str1 = str2 returns 0
 *         str1 > str2 returns 1
 * NOTE: Regards \n OR \0 as string delimeters.
 */
int compare(char* str1, char* str2) {
    //printf("\ncomparing '");
    //printf(str1);
    //printf("' and '");
    //printf(str2);
    //printf("'.");
    /* Compare each character from the beginning,
       returning the result of the first difference. */
    while(1) {
        if(*str1 == *str2) {
            /* If we haven't reached the end yet, increment. */
            if(*str1 && *str1 != '\n') {
                ++str1;
                ++str2;
                continue;
            } else {
                return 0;
            }
        }
        /* Also let \n be a string delimiter, so I can reuse this method. */
        if((!*str1 || *str1 == '\n') && (!*str2 || *str2 == '\n'))
            return 0;
        if(*str1 < *str2)
            return -1;
        if(*str1 > *str2)
            return 1;
    }
}

/* Writes to indexedDict the pointers to each word in a dictionary. */
void indexDictionary(char* dictionary) {
    int i = 0;
    /* Clear out the buffer lazily,
     * assuming that all \0's are contiguous with the end. */
    while(indexedDict[i]) { indexedDict[i++] = NULL; }
    i = 0;
    //printf("\nIndexing...");
    while(*dictionary && *dictionary == '\n') { ++dictionary; }
    while(*dictionary) {
        /* Assign beginning of new word into indexedDict */
        indexedDict[i++] = dictionary;
        /* Walk through the non-\n characters */
        while(*dictionary && *dictionary != '\n') { ++dictionary; }
        /* Walk through the \n character(s) */
        while(*dictionary && *dictionary == '\n') { ++dictionary; }
    }
    //printf("\nDone indexing.");
}

/* Debugging function that prints the indexed dict. */
void printIndexedDict(char* indexedDictionary[BUFFER_SIZE2]) {
    int i = 0;
    int j = 0;
    while(indexedDictionary[i][0]) {
        j = 0;
        putchar('<');
        while(indexedDictionary[i][j] != '\n' &&
              indexedDictionary[i][j]) {
            putchar(indexedDictionary[i][j++]);
        }
        putchar('>');
        putchar('\n');
        /* Exit if the next pointer is NULL. */
        if(!indexedDictionary[++i]) {
            break;
        }
    }
}

/* Return dictionary size (number of words in dictionary). */
int indexedDictLen(char* indexedDictionary[BUFFER_SIZE2]) {
    int i = 0;
    while(indexedDictionary[i]) { ++i; }
    return i;
}

/* Selection sort the given array of strings.
 * NOTE: Not needed. Here in case assumptions change.
 */
void sortIndexedDict(char* indexedDictionary[BUFFER_SIZE2]) {
    int i, j;
    int min;
    int length = indexedDictLen(indexedDictionary);
    char* tmpWord;
     
    //printf("\nSorting...\n");
    for(j = 0; j < length-1; j++) {
        min = j;
        //printf("\nNEXT %d\n", j);
        for(i = j+1; i < length; i++) {
            //printf("\nCMP %d\n", i);
            if(compare(indexedDictionary[i], indexedDictionary[min]) == -1) {
                min = i;
            }
        }
        /* min is index of minimum element.
         * Swap it with current position j.
         */
        if(min != j) {
            //printf("\nSWAP\n");
            tmpWord = indexedDictionary[j];
            indexedDictionary[j] = indexedDictionary[min];
            indexedDictionary[min] = tmpWord;
        }
    }
    //printf("Done sorting.");
}

/* Checks if a given key is in the dictionary.
 * Uses deferred equality detection iterative binary search.
 */
bool inDict(char* key, char* indexedDictionary[BUFFER_SIZE2]) {
    int min = 0;    
    int max = indexedDictLen(indexedDictionary);
    int mid = 0;
    //printIndexedDict(indexedDictionary);
    while(min < max) {
        mid = (min+max)/2;
        //printf("\n\tmin:%d mid:%d max:%d", min, mid, max);
        /* Guarantee interval reduced each iteration */
        //if(!(mid < max)) {
        //    printf("ERROR");
        //}
        if(compare(indexedDictionary[mid], key) == -1) {
            min = mid+1;
        } else {
            max = mid;
        }
    }
    /* Key is now at min or not in dict. */
    return((max == min) && (compare(indexedDictionary[min], key) == 0));
}

void spellCheck(char article[], char dictionary[]) {
    char* nextWord;
    int wordLen;
    int i = 0;
    toLower(article);
    toLower(dictionary);
    indexDictionary(dictionary);
    //printf("Dictionary:\n\"\"\"\n%s\n\"\"\"\n", dictionary);
    /* If the dictionary is not sorted, sort it. */
    //sortIndexedDict(indexedDict);
    //printIndexedDict(indexedDict);
    /* Get rid of prefixed garbage. */
    while(*article && !isLetter(*article)) { ++article; }
    while(*article) {
        //printf("Testing article:\n\t\"\"\"%s\"\"\"\n", article);
        wordLen = 0;
        /* Save the new word's position. */
        nextWord = article;
        /* Measure the word's length. */
        while(isLetter(*article)) { ++wordLen; ++article; }
        /* Walk to the next word. */
        while(*article && !isLetter(*article)) { ++article; }
        /* Take care of 1 letter words. */
        if(wordLen < 2) { continue; }
        /* Null-terminate the word. */
        *(nextWord+wordLen) = '\0';
        //printf("Dict search: '%s'\n", nextWord);
        /* Search for the word in the indexed dictionary. */
        if(!inDict(nextWord, indexedDict)) { 
            /* Print the word if is not in the given dictionary. */
            printf(nextWord);
            putchar('\n');
        }
    }
}
