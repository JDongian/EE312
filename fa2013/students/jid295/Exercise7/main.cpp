#include <stdio.h>
#include "Vector.h"


void printVector(Vector param) {
	const char* prefix = "";
	for (uint32_t k = 0; k < param.size(); k += 1) {
		printf("%s%d", prefix, param[k]);
		param[k] += 10;
		prefix = ", ";
	}
	printf("\n");
}




bool test1(void) {
	bool pass = true;
	Vector x(10);

	for (int k = 0; k < 10; k += 1) {
		x[k] = k;
	}

	for (int k = 0; k < 10; k += 1) {
		if (x[k] != k) { pass = false; }
	}

	return pass;
}

int main(void) {
	printf("test1: ");
	if (test1()) { printf("passed\n"); }
	else { printf("failed\n"); }

	/* write other tests and call them from here */
}


