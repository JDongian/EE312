#include <stdio.h>
#include <stdint.h>

/*
 * this file contains an implementation of selection sort
 * The implementation is likely to contain errors (i.e. bugs) and probably won't 
 * actually sort things correctly. Your job is to determine what errors (if any) exist
 * and fix them.
 */

/*
 * swap the values stored in memory locations p and q
 */
void swap(int32_t* p, int32_t* q) {
	int32_t tmp = *p;
	*p = *q;
	*q = tmp;
}


/*
 * search through x and find the minimum element return a pointer to that element. 
 * If there are two (or more) equally small minimal elements, return a pointer to any one of them
 */
int32_t* min(int32_t x[], int n) {
	/* standard "min" idiom, with one minor change
	 * use a pointer to the guess, and update (and return) the pointer rather than the value */

	int32_t* result = &x[0]; // guess the first one is smallest
	int k = 1;
	while (k < n) {
		if (x[k] < *result) { result = &x[k]; }
		k = k + 1;
	}
	return result;
}

/* selection sort:
 * find the smallest element, and put that in the first position (by swapping the element that is currently
 * first with the minimum element).
 * then, find the second-smallest element, and put that second.
 * find the third-smallest element, and put that third, and so on */
void sort(int32_t x[], int n) {
	for (int k = 0; k < n; k += 1) {
		int32_t* next_min = min(x + k, n-k);
		swap(&x[k], next_min);
	}
}


void printArray(int32_t x[], int n) {	
	char* prefix = "";

	for (uint32_t k = 0; k < n; k += 1) {
		printf("%s%d", prefix, x[k]);
		prefix = ", ";
	}

}

int main(void) {
	int test[] = { 1, 6, 2, 3, 9, 4, 8, 7, 5, 0 };
	sort(test, 10);
	printArray(test, 10);
}

