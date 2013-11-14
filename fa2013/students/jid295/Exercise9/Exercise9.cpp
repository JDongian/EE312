#include <stdio.h>
#include <stdint.h>
#include <vector>
#include "HashTable.h"

using std::vector;

/*********************************************
 * Project 9 Part One
 * Write as many functions as you need to compile the statistics of all six
 * combinations of hashcodes and table indexing methods (see the PDF)
 *
 * You can ignore brokenHash and stupidHash if you wish
 */

extern vector<String> words; // this vector is a global variable defined in main.cpp (I'm declaring it here)
void checkBasicSimple(const vector<String>&); // checks the stats for one of the six combos required for part one (included below)
void checkKnuthSimple(const vector<String>&); // checks the stats for one of the six combos required for part one (included below)

void myPartOne(void) {
	checkBasicSimple(words);
	checkKnuthSimple(words);
}

unsigned brokenHash(String s) { return 42; }

unsigned stupidHash(String s) { return s[0]; }

unsigned simpleHash(String s) {
	unsigned h = 0; 
	for (int k = 0; k < s.size(); k += 1) {
		h += s[k];
	}
	return h;
}

unsigned smartHash(String s) {
	unsigned h = 0; 
	for (int k = 0; k < s.size(); k += 1) {
		h *= 33;
		h += s[k];
	}
	return h;
}

void printStats(const vector<int>& x) {
	/* find the mean and max */
	double sum = 0.0;
	int max = 0;
	for (int k = 0; k < x.size(); k += 1) {
		sum += x[k];
		if (x[k] > max) { max = x[k]; }
	}
	double mean = sum / x.size();

	/* find the variance */
	double vsum = 0.0;
	for (int k = 0; k < x.size(); k += 1) {
		double t = x[k] - mean;
		vsum += t * t;
	}
	double variance = vsum / x.size();
	double std_dev = sqrt(variance);

	printf("Max was %d, mean was %g and standard deviation was %g\n",
		max, mean, std_dev);
}

unsigned modIndex(unsigned hash, unsigned tsize) {
	return hash % tsize;
}

unsigned KnuthIndex(unsigned hash, unsigned tsize) {
	int log2Size = 0; // the log (base 2) of tsize
	while (tsize > 1) { 
		log2Size += 1; 
		tsize /= 2; 
	}
	return (hash * 0x6b43a9b5) >> (32 -log2Size);
}

/* this is a template for checking one of our six pairings of HashFunction (simple, or smart)
 * and array indexing (mod 64K, mod Prime, or Knuth Multiplication). 
 * This happens to be for the pairing of Knuth Multiplication with simple hash */
void checkKnuthSimple(const vector<String>& words) {
	vector<int> histoGram(65536);
	for (int k = 0; k < histoGram.size(); k += 1) { histoGram[k] = 0; }

	for (int k = 0; k < words.size(); k += 1) {
		unsigned h = simpleHash(words[k]);
		unsigned p = KnuthIndex(h, 65536);
		histoGram[p] += 1;
	}

	printf("results for SimpleHash with Knuth multiplication:\n");
	printStats(histoGram);
	printf("\n");
}

/* this is a template for checking one of our six pairings of HashFunction (simple, or smart)
* and array indexing (mod 64K, mod Prime, or Knuth Multiplication).
* This happens to be for the pairing of Mod 64K with simple hash */
void checkBasicSimple(const vector<String>& words) {
	vector<int> histoGram(65536);
	for (int k = 0; k < histoGram.size(); k += 1) { histoGram[k] = 0; }

	for (int k = 0; k < words.size(); k += 1) {
		unsigned h = simpleHash(words[k]);
		unsigned p = h % 65536;
		histoGram[p] += 1;
	}

	printf("results for SimpleHash with Mod 64K\n");
	printStats(histoGram);
	printf("\n");
}

/******************************
 * Project 9 Part 2
 * Write HashTable::resize()
 *//////////////////////////////

void HashTable::resize(void) {
}