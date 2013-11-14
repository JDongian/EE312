#include <stdio.h> // declarations of fopen and fgets
#include <vector>
#include <math.h>
#include "String.h"
#include "HashTable.h"

using std::vector;

char dictionary[1000000];


vector<String> words; // words is a collection of words built using the dictionary from Project 1

void myPartOne(void); // your function, it's in Project9.cpp

void partOne(void) {
	myPartOne(); // you write this function, it's in Project9.cpp
}

void partTwo(void) {
	HashTable table;
	for (int k = 0; k < words.size(); k += 1) {
		table.insert(words[k]);
	}

	for (int k = 0; k < words.size(); k += 1) {
		if (! table.isMember(words[k])) {
			printf("oops! %s not found in the hash table\n", words[k].c_str());
			break;
		} else {
			if (k < 20) {
				printf("Good, %s is in the table\n", words[k].c_str());
			} else if (k == 20) {
				printf("Printing all these words is getting boring.\nI'll just check the rest and let you know if there's a problem\n");
			}
		}
	}
}

int main(void) {
	FILE* dict_file = fopen("american-english.txt", "r");
	if (dict_file == 0) {
		printf("unable to open dictionary file \"american-english.txt\"\n");
		return -1;
	}
	/* read dictionary */
	int result = fread(dictionary, sizeof(char), sizeof(dictionary), dict_file);

	char* p = dictionary;
	while (*p != 0) {
		char* word_start = p;
		while (*p != 0 && *p != '\n') { p += 1; }
		if (*p == '\n') { *p = 0; p += 1; }
		words.push_back(String(word_start));
	}
	printf("looks like I found %d words in that dictionary\n", words.size());

	partOne();
	partTwo();

}

