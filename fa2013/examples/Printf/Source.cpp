
typedef unsigned uint32_t;
typedef int int32_t;

extern "C" { void putchar(int); }

void displayDecimal(int32_t x) {
	char digits[10];
	uint32_t num_digits = 0;

	if (x == 0) { // special case for zero
		putchar('0');
		return;
	}

	if (x < 0) { // special case for negative
		putchar('-');
		x = -x;
		// fall through for positive case
	}

	while (x != 0) {
		int32_t digit = x % 10;
		x = x / 10;
		digits[num_digits] = digit + '0';
		num_digits += 1;
	}

	while (num_digits > 0) {
		num_digits -= 1;
		putchar(digits[num_digits]);
	}
}

void myPrintf(char fmt[], ...) {
	int32_t k = 0;

	int32_t* p = &k;
	p = p + 4;

	while (fmt[k] != 0) {
		if (fmt[k] == '%') {

			if (fmt[k+1] == 'd') {
				int32_t x = *p;
				p = p + 1;
				displayDecimal(x);
			} else if (fmt[k+1] == 'c') {
				int32_t x = *p;
				p = p + 1;
				putchar(x);
			} else if (fmt[k+1] == 's') {
			} else { 
				// WTF???
			}

			k += 2; 
		} else {
			putchar(fmt[k]);
			k += 1;
		}
	}

}

int main(void) {
	myPrintf("Hello World\n");
	myPrintf("the answer is %d ASCII for 65 is %c\n", 42, 65);
}
