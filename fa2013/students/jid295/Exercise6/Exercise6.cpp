#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Problem 1: Constructors */
struct Complex {
	double real;
	double imag;
	Complex(void);
	Complex(double);
	Complex(double, double);

	void operator+=(Complex);
	void operator-=(Complex);
};

Complex::Complex(void) { // Constructor #1
	real = 0.0;
	imag = 0.0;
}

Complex::Complex(double r) { // Constructor #2
	real = r;
	imag = 0.0;
}

Complex::Complex(double r, double i) { // Constructor #3
	real = r;
	imag = i;
}

Complex addition(Complex, Complex); // this function is defined below

/* for problem 1, you should add Constructor calls to the definitions of variables 
 * x, y, and z in the function below. Use each of the three constructors one time
 * (as called for by the comments)
 *
 * In addition, you must write the function "addition". However, your solution
 * must contain only a single return statement. You are permitted to invoke a constructor
 * in the return statement, but you cannot declare any local variables or have any other statements
 * in your function (just the return).
 */
void problem1(void) {
	Complex x; // Initialize x using Constructor #1
	Complex y; // Initialize y using Constructor #2
	Complex z; // Initialize z using Constructor #3

	Complex t = addition(Complex(1, 2), Complex(10, 11));
	assert(t.real == 11 && t.imag == 13);
}

/* write this function using only a single return statement.
 * do not declare any local variables that creates a Complex struct 
 * which represents the mathematical result of adding x and y */
Complex addition(Complex x, Complex y) {
}



/*
 * Problem 2
 * Constructors with malloc and destructors
 */

struct String {
	char* ptr;
	String(char* src);
	~String(void);
};

/* initialize the String struct so that it has a pointer to an array on the heap
 * populate the array to be an exact copy of the string src (including the zero on the end)
 * You may use strlen and strcpy for this constructor if you'd like (although I still recommend
 * doing the function from scratch, it ain't that hard).
 */
String::String(char* src) {
	/* make ptr point to an array allocated from the heap
	 * and fill in that array with an exact copy of the string src (which is an ordinary C string) */
}

/* write a destructor for the String struct
 * your destructor will ensure there are no memory leaks. */
String::~String(void) {
	printf("the string %s is going away now\n", ptr); // please leave this print statement in the destructor
}

/*
 * examine the code below before running it. make a prediction of what you think the output will be
 * run the program (to see the actual output) 
 * provide in the space below your best explanation as to why the function produces the output it does
 * (please put your answer in these comments, replacing the %d and %s with the results of your analysis
 * -- and yes, that's C humor, pretty lame I know)
 * PREDICTION: 
 *    I expected to see %d destructor print messages called because %s
 *
 *
 *
 *
 * ACTUAL:
 *    The program produecs %d destructor messages in the following order %s %s %s.... %s
 *    The reason there are %d destructor messages is:
 *    The reason the messages are in the order they are is because apparently C++ likes to destroy
 *    objects in %s order
 */
void problem2(void) {
	String s1 = String("first string");
	String s2 = String("second string");

	for (int k = 0; k < 5; k += 1) {
		String s3 = String("third string");
	}
}

/* Problem 3: references (easy), plus copy/copy constructor (deep)
 * please skip down to the function problem3(void) to see the question
 */

void doubleDeleteBugOhMy(String s) {
	printf("the string %s has been used as an argument to this function\n", s.ptr);
}

void noBugWhyNot(String* s) {
	printf("the string %s has been used as an argument to this function\n", (*s).ptr);
}

void bugHereOrNot(String& s) {
	printf("the string %s has been used as an argument to this function\n", s.ptr);
}

/*
 * In the code below, we have three invocations of virtually identical functions
 * The only difference between these functions is the type of parameter used
 * The first function uses an ordinary struct 
 * The second function uses a pointer to a struct
 * The third function uses a reference to a struct
 *
 * No matter which function is called, everything in problem3 seems fine right up until
 * the problem3 function returns to main. 
 *
 * If function 1 is called, we have a "double delete" memory error (double delete is a special
 * case of the more general "use after free bug"). Explain why this error occurs in the space below
 * doubleDeleteBugOhMy creates a double delete because:
 *
 * 
 * 
 * If function 2 is called, there are no memory errors whatsoever explain why function 2 does
 * not suffer from the same double-delete as function 1
 *
 *
 *
 * When function 3 is called, is there also be a double delete? If so, why? If not, why not?
 *
 *
 *
 *
 */
void problem3(void) {
	String s = String("hello world");

	/* uncomment the function calls below one at a time, and answer the questions posed in the comments
	 * above this function (please put your answer in the space provided in the comments, use additional 
	 * lines if necessary) */

	// doubleDeleteBugOhMy(s); // note, if this function is called, problem3 crashes when it returns to main!
	// noBugWhyNot(&s);
	// bugHereOrNot(s);
}


/*
 * Problem 4 operator overloading
 *
 * Since you already wrote an addition function for Complex numbers (in problem 1) we can use
 * that function to provide an addition operator for expressions like "x + y" when x and y are Complex
 * objects. I've provided an example for operator+ below. You are to write four more operators
 *
 * operator-(Complex); // negation (i.e., "unary minus") -- this function should not be a member function
 * operator-(Complex, Complex); // binary subtration (i.e., normal minus) -- not a member function
 * operator+=(Complex); // addtion for expressions like x += y -- must be a member function
 * operator-=(Compelx); // subtraction for expressions x -= y -- must be a member function
 *
 * note, to write the two member functions, you will need to modify the struct Complex
 * defined at the top of this file. 
 */

Complex operator+(Complex x, Complex y) { return addition(x, y); }
Complex operator-(Complex x) { /* your code here */ }
Complex operator-(Complex x, Complex y) { /* your code here */ }

void Complex::operator+=(Complex y) { /* your code here */ }
void Complex::operator-=(Complex y) { /* your code here */ }

void problem4(void) {
	Complex x = Complex(1.0);
	Complex y = x - Complex(0.0, 1.0); // y is 1-i
	Complex z = -y + x; // z is i

	y += z; // y is 1
	x -= y; // x is zero

	z = z + z; // 2i

	/* now, check your math */
	assert(fabs(x.real) < 0.001 && fabs(x.imag) < 0.001);
	assert(fabs(y.real - 1.0) < 0.001 && fabs(y.imag) < 0.001);
	assert(fabs(z.real) < 0.001 && fabs(z.imag - 2.0) < 0.001);

	printf("problem4 arithmetic passed the test (well, this *very* simple test)\n");
}


/* Problem 5 member functions (part 1) */
struct Foo {
	int x;
	void doit(void); 
	void doit2(Foo* that);
	void doit3(void);
	void doit4(Foo* that);
};
Foo f;
Foo g;
Foo h;

/*
 * problem 5 is mostly illustration on how member functions use the implicit parameter "this"
 * 
 * To solve this problem, simply (a) change the way that doit2 is invoked so that f.x is set to 20
 * and g is set to 10
 * and then (b) call doit4 in such a way that all of the elements in anArrayOfFoo are initialized to 42
 * NOTE: you are only allowed to call doit4 one time (no loops!)
 */
void problem5(void) {
	f.x = 1;
	g.x = 2;

	f.doit();
	printf("f is %d and g is %d\n", f.x, g.x);

	f.doit2(&g);
	printf("f is %d and g is %d\n", f.x, g.x);

	if (f.x == 20 && g.x == 10) { printf("you win!\n"); }
	else { printf("try again\n"); }

	Foo anArrayOfFoo[10];
	/* insert one call to doit4 here that initializes all the elements of the array to 42 */
	anArrayOfFoo[0].doit4(anArrayOfFoo + 10);

	/* insert your call to doit4 above (i.e., before) this line */
	bool failure = false;
	for (int k = 0; k < 10; k += 1) {
		if (anArrayOfFoo[k].x != 42) {
			failure = true;
			printf("looks like doit4 failed to do the trick, try again\n");
		}
	}
	if (! failure) { printf("hey! pretty good trick with doit4 there\n"); }
	
}

void Foo::doit(void) {
	if (this == &f) { printf("doit called with this == &f\n"); }
	else if (this == &g) { printf("doit called with this == &g\n"); }
	else { printf("what's up with this?"); }
	x = 42;
}

void Foo::doit2(Foo* that) {
	if (this == &f) { printf("doit2 called with this == &f\n"); }
	else if (this == &g) { printf("doit2 called with this == &g\n"); }
	else { printf("what's up with this?"); }

	if (that == &f) { printf("doit2 called with that == &f\n"); }
	else if (that == &g) { printf("doit2 called with that == &g\n"); }
	else { printf("what's up with that?"); }

	this->x = 10; 
	that->x = 20;
}

void Foo::doit4(Foo* that) {
	x = 42;
	if (this == that) { 
		printf("the end\n");
	} else {
		(this + 1)->doit4(that);
	}
}


int main(void) {
	problem1();
	//problem2(); // problems 2 and 3 are optional, good study for the exam, but not part of today's exercise
	//problem3();
	problem4();
	problem5();
}
