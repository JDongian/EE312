/*
 * Project4.c
 *
 * Joshua Dong
 * Chris
 * F
 *
 *
 */


#include <stdio.h>
#include "MazeParams.h"
#define MIN_DOUBLE 1e-15
#define VALUE_PENNY 1
#define VALUE_NICKLE 5
#define VALUE_DODEK 12

/* return the smallest of the elements in array x[]
 * there are n elements in x[] (x[0].. x[n-1])
 */
int minIt(int x[], int n) {
    int minimum = x[0];
    for (int i = 0; n > i; ++i) {
        if (x[i] < minimum) {
            minimum = x[i];
        }
    }
    return minimum;
}

/* return the smallest of the elements in array x[]
 * there are n elements in x[] (x[0].. x[n-1])
 * solve the problem recursively and 
 * use an "n-1" type of decomposition
 */
int minRec1(int x[], int n) {
    if (n == 2) {
        if (x[0] < x[1]) {
            return x[0];
        } else {
            return x[1];
        }
    }
    int remaining = minRec1(x, n-2);
    if (x[n-1] < remaining) {
        return x[n-1];
    } else {
        return remaining;
    }
}

/*
 * return the smallest of the elements in array x[]
 * there are n elements in x[] (x[0].. x[n-1])
 * n may be either odd or even
 * solve the problem recursively and 
 * use an "n / 2" type of decomposition
 */
int minRec2(int x[], int n) {
    if (n == 1) {
        return x[0];
    } else if (n == 2) {
        if (x[0] < x[1]) {
            return x[0];
        } else {
            return x[1];
        }
    }
    int left = minRec2(x, (n-1)/2);
    int right = minRec2(x+(n-1)/2, n-(n-1)/2);
    if (left > right) {
        return right;
    } else {
        return left;
    }
}

/*
 * calculate and return the square root of x.
 * The other two parameters are estimates of the square root.
 * low_guess is smaller than the actual square root, and high_guess 
 * is larger than the actual square root.
 * Searching for the square root can be done much like searching
 * for a name in a phone book.
 *
 * Since you can't calculate a square root exactly, for this problem
 * you're required to calculate the answer to 15 decimal digits of
 * accuracy.
 */
double sqrtIt(double x, double low_guess, double high_guess) {
    return 0;
}

/*
 * calculate and return the square root of x.
 * The other two parameters are estimates of the square root.
 * low_guess is smaller than the actual square root, and high_guess 
 * is larger than the actual square root.
 * Searching for the square root can be done much like searching
 * for a name in a phone book.
 *
 * Since you can't calculate a square root exactly, for this problem
 * you're required to calculate the answer to 15 decimal digits of
 * accuracy.
 */
double sqrtRec(double x, double low_guess, double high_guess) {
    /* Please no negative please please */
    if (high_guess <= 0 or x <= 0) {
        printf("Out of domain.\n");
        return 0;
    }
    double mid_guess = (low_guess+high_guess)/2;
    double mid_result = x/mid_guess;
    /* If accuracy is good enough, return. */
    if (((high_guess - low_guess)/low_guess) < MIN_DOUBLE) {
        return mid_guess;
    }
    /* If mid_guess lowers the max bound and
     * mid raises the min bound.
     */
    if (mid_result < mid_guess) {
        //printf("low=%f, %f (%f), high=(%f)\n", low_guess, mid_guess, mid_result, high_guess);
        return sqrtRec(x, low_guess, mid_guess); 
    }
    /* If mid_guess raises the min bound and
     * mid lowers the max bound.
     */
    if (mid_result > mid_guess) {
        //printf("high=%f, (%f) %f, low=(%f)\n", high_guess, mid_guess, mid_result, low_guess);
        return sqrtRec(x, mid_guess, high_guess);
    }
    printf("Error.\n");
}


/*
 * using only recursion, write a string comparison function
 * return -1 if str1 is less than str2
 * return 0 if the two strings are equal
 * return +1 if str1 is greater than str2
 * when comparing strings, use the ASCII value to compare each character
 * (i.e., that means 'A' is less than 'a' since it's ASCII value is smaller)
 * The string str1 is less than str2 if either
 * str1[0] < str2[0]
 * or there exists a k such that str1[k] < str2[k] and 
 *   for all j < k str1[j] == str2[j]
 *   and k is less than the length of str1 and str2
 */

int strCompare(char* str1, char* str2) {
    if (*str1 && *str2) {
        /* If str1 is less than str2 */
        if (*str1 < *str2) {
            return -1;
        }
        /* If str1 is greater than str2 */
        if (*str1 > *str2) {
            return 1;
        }
        return strCompare(str1+1, str2+1);
    }
    if (!*str1) {
        return -1;
    }
    if (!*str2) {
        return 1;
    }
    return 0;
}

/*
 * if c is not a letter return -1
 * if c is a letter, return the position of c in the alphabet 
 * where 'A' (and 'a') are position 1 and 'Z' (and 'z') are position 26
 *
 * This code is rather ugly as I'm exploiting some detailed knowledge of the ASCII table
 */
int whatLetter(char c) {
    if (c < 'A') { return -1; }
    if (c > 'z') { return -1; }
    if (c > 'Z' && c < 'a') { return -1; }
    return c & ~32 - 64;
}

/*
 * same as before, only this time 
 * ignore anything that is not a letter
 * ignore the case (upper case, lower case)
 * So, strCompare2("Hello", "hello") should return 0 (they're the same)
 * strCompare2("The plane!", "theater") should return 1 since "theplane" is larger than "theater"
 * once again, you can only use recursion, no loops
 */
int strCompare2(char* str1, char* str2) {
    char char1 = whatLetter(*str1);
    char char2 = whatLetter(*str2);
    if (!*str1 || !*str2) {
        if (*str2 && (char2 != -1)) { return -1; }
        if (*str1 && (char1 != -1)) { return 1; }
        return 0;
    }
    if (char1 == -1 || char2 == -1) {
        if (char1 == -1) { ++str1; }
        if (char2 == -1) { ++str2; }
        return strCompare2(str1, str2);
    }
    if (char1 < char2) {
        return -1;
    }
    if (char1 > char2) {
        return 1;
    }
    return strCompare2(++str1, ++str2);
}

/* return false if there is a wall in the square for row and col
 * return true if it's not a wall.
 */
int isOK(int row, int col) {
    return (row > 0 && row < MATRIX_SIZE
            && col > 0 && col < MATRIX_SIZE
            && maze[row][col] == 0);
}

/* Blah blah blah
 */
int solveMazeRec(int row, int col) {
    maze[row][col] = 2;   
    if (row+1 == MATRIX_SIZE) {
        return 1;
    }
    /* ⇧  */
    if (isOK(row-1, col) && solveMazeRec(row-1, col)) {
        return 1;
    }
    /* ⇨  */
    if (isOK(row, col+1) && solveMazeRec(row, col+1)) {
        return 1;
    }
    /* ⇩  */
    if (isOK(row+1, col) && solveMazeRec(row+1, col)) {
        return 1;
    }
    /* ⇦  */
    if (isOK(row, col-1) && solveMazeRec(row, col-1)) {
        return 1;
    }
    /* ☹  */
    maze[row][col] = 0;
    return 0;
}

/* Herp Blah Derp
 */
void adjacentCell(int row, int col, int dir, int* trow, int* tcol) {
    *trow = row;
    *tcol = col;
    switch(dir) {
        case 0: // UP
            *trow = *trow - 1;
            break;
        case 1: // RIGHT
            *tcol = *tcol + 1;
            break;
        case 2: // DOWN
            *trow = *trow + 1;
            break;
        case 3: // LEFT
            *tcol = *tcol - 1;
            break;
    }
}

/*
 * return the value of the direction that is one turn to the right
 */
int turnRight(int dir) {
    return (dir + 1) % 4;
}

/*
 * return the value of the direction that is one turn to the left
 */
int turnLeft(int dir) {
    return (dir + 3) % 4;
}

/*
 * the two dimensional array maze[MATRIX_SIZE][MATRIX_SIZE] contains a maze
 * Each square is (initially) either a 1 or a zero.  Each 1 represents a wall
 * (you cannot walk through walls, so you can't go into any square where the value
 * is 1).  Each 0 represents an open space.  
 *
 * write an iterative solution to find your way out of the maze
 * Your starting point is at row and col. (params to this function)
 * Your exit point is somewhere in the last row (which is: MATRIX_SIZE - 1)
 * The maze can be solved by following the right hand wall.  In fact, there
 * is exactly one path between any two points in the maze (well, between any two
 * points that are not walls).
 *
 * The algorithm is not too bad, although it is certainly not trivial
 * Initially, you'll be headed DOWN (direction 2)
 * Each iteration do one of the following.  Note that only one of two cases
 * can possibly apply (the properties of the maze guarantee that).
 *    case 1: you can move in the current direction
 *       In this case, you should take one step in the current direction
 *       and then turn right.  
 *    case 2: you cannot move in the current direction
 *       If there's a wall directly in front of you, then to follow the right
 *       hand wall, you'd need to turn left (placing your hand on the wall that
 *       used to be directly in front of you).  So, if you discover this case
 *       then turn left.  Don't move, just turn left.
 * If you were walking down a straight corridor (with walls on both sides)
 * then you'd alternate case 1 and case 2.  On the first iteration, you'd
 * take a step, and then turn right (case 1).  This causes you to walk
 * one position down the hallway but then turn to face the wall.
 * On the next iteration, you'll be facing the wall, so you can't walk
 * forward and therefore (case 2) you'll turn left.  On the third iteration
 * you'll be able to walk down the hallway again.
 *
 * If you follow those two cases, then you'll eventually find your way out
 * of the maze.  To confirm that you're making it out, leave a "bread crumb" 
 * behind each square along the path to the exit.
 * For partial credit, place a bread crumb in every square you visit.
 * For full credit, pick up all the breadcrumbs when you backtrack.  
 * Backtracking is when you go back to a square you've already been before.  
 * If the square you're about to step into has a breadcrumb, then pick up 
 * the bread crumb in the square you're at now.
 * You indicate "bread crumbs" by setting the square equal to "2"
 */
void solveMazeIt(int row, int col) {
   // int dir = 2; // 0 is up, 1 is right, 2 is down, 3 is left.
   // maze[row][col] = 2; // drop a bread crumb in the starting square
   // while (row < MATRIX_SIZE - 1) { // the exit is the only open square 
   //     // in the last row

   //     /* the rest of this loop is yours */

   // }
}


/*
 * This struct is used in part5, martian currency
 * A Martian indicates a collection of coins. The components indicate the number of pennies
 * nicks and dodeks in that collection. The value (in cents) of a Martian is the weighted sum
 * i.e., if you had two pennies, three nicks and four dodeks, you'd have 2 + 15 + 48 = 65 cents
 */

struct Martian {
    int pennies;
    int nicks;
    int dodeks;
};

int valueOf(Martian combo) {
    return combo.pennies + combo.nicks*VALUE_NICKLE + combo.dodeks*VALUE_DODEK;
}
/*
   * returns the number of coins represented in a Martian
   */
int coins(Martian combo) {
    return combo.pennies + combo.nicks + combo.dodeks;
}

/*
   * calculates the optimal change for a given amount of money
   */
Martian change(int cents) {
    Martian c, nick_combo, t_combo;
    /* Special case */
    if (cents == 0) { return { 0,0,0 }; }
    /* sort of special case */
    if (cents > 21) {
        t_combo = change(cents - 12);
        ++t_combo.dodeks;
        return t_combo;
    }
    c = change(cents - 1);
    ++c.pennies;
    if (cents >= 5) {
        nick_combo = change(cents - 5);
        ++nick_combo.nicks;
    } else { return c; }
    if (cents >= 12) {
        t_combo = change(cents - 12);
        ++t_combo.dodeks;
    } else { return nick_combo; }
    if (valueOf(nick_combo) == cents && valueOf(t_combo) == cents) {
        if (coins(nick_combo) < coins(t_combo)) {
            return nick_combo;
        } else { return t_combo; }
    } else { return t_combo; }
}
