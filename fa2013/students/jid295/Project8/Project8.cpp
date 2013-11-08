#include <stdio.h>
#include <stdint.h>

FILE* input_stream;

bool readNum(int& x) {
	return EOF != fscanf(input_stream, "%d", &x);
}

int main(void) {
	input_stream = fopen("input.txt", "r");

}


