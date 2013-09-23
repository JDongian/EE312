/*
 * Project7.c
 *
 * Joshua Dong
 * Steven
 * My Section Time
 *
 * Your Mission:
 * Stage 1:
 *   write function multiplyMatrices
 * Stage 2:
 *   write functions allocateSpace and multiplyMatrixChain
 * Stage 3: 
 *   fix any bugs that you missed during the first two stages
 */

#include <stdio.h>
#include <stdint.h>
#include <assert.h> 
#include <math.h>
#include "MatrixMultiply.h"


/*
 * An example routine demonstrating the use of row-major ordering
 *
 * You do not need to use this function in your solution 
 * (I didn't use it in mine!)
 *
 * given two matrices a and b, where the number of rows in b is equal
 * to the number of columns in a,
 * and given a row number 'row' and a column number 'col', 
 * calculate the dot product of the row'th row from a and the col'th column from b
 * 
 * recall the dot product is the sum of the element-by-element product of 
 * the two vectors.
 */
double dotProduct(double a[], const uint32_t a_rows, const uint32_t a_cols,
        /* a is a matrix with a_rows rows and a_cols columns */
        double b[], const uint32_t b_cols,
        /* b is also a matrix.  It has a_cols rows and b_cols columns */
        uint32_t row, // an index of a row of a
        uint32_t col) { // an index of a column of b

    double sum = 0.0; // the result of the dot product is stored here
    uint32_t k = 0;
    while (k < a_cols) { // recall: a_cols == b_rows
        /* we need to multiply a[row, k] and b[k, col] and add that to sum */
        sum = sum + a[(row * a_cols) + k] * b[k * b_cols + col];
        /* recall a[i,j] is stored at a[(i * a_cols) + j] and b[i, j] is at b[(i * b_cols) + j] */
        k += 1;
    }

    return sum;
}

/*
 * multiply matrices a and b.  do not change the contents of a or b.
 * place the result in c.
 * a has a_rows rows and a_cols columns.
 * b has a_cols rows and b_cols columns
 * c has a_rows rows and b_cols columns.
 *      You do not need to check the array sizes, your function should just
 *          assume that a, b and c are the correct sizes.
 * (you couldn't really check anyway, this is C++ after all).
 * all matrices are stored in row-major order.
 * in other words, a[i,j] is stored at a[(i * a_cols) + j], similarly for b and c.
 * 
 * Notice to those people who know enough to be dangerous:
 * Do not attempt to use the parameters as arrays of arrays e.g., 
 * it is incorrect to rewrite this function with a[i][j] and b[i][j].  You must
 * explicitly use row-major ordering and one-dimensional arrays.
 */
void multiplyMatrices(double a[], const uint32_t a_rows, const uint32_t a_cols,
                      double b[], const uint32_t b_cols, // b_rows == a_cols
                      double c[]) { // c_rows = a_rows, c_cols = b_cols 
    double sum;
    uint32_t row, col, i;
    for (row = 0; row < a_rows; ++row) {
        for (col = 0; col < b_cols; ++col) {
            sum = 0;
            for (i = 0; i < a_cols; ++i) {
                sum += a[row*a_cols+i] *
                       b[i*b_cols+col];
            }
            c[row*b_cols+col] = sum;
        }
    }
}

#define READY_FOR_STAGE2
#ifdef READY_FOR_STAGE2

/* these three variables are used in Stage 2 to implement allocateSpace */
#define TOTAL_SPACE 10000
double memory_pool[TOTAL_SPACE];
uint32_t top_of_pool = 0;

/*
 * allocate space from the memory_pool array
 * allocate sufficient space to hold 'size' variables of type double
 * return a pointer to the first of these doubles
 * update the top_of_pool variable so that we will not accidentally
 * use the same space again.
 */
PtrDouble allocateSpace(uint32_t size) {
    top_of_pool += size;
    /* Perhaps take advantage of CPU caching???? */
    return memory_pool+top_of_pool-size;
}

/*
 * given an array of matrices, 'matrixList'
 * compute the product of the matrices and store the result in the
 * matrix out.
 * 
 * You should use the multiplyMatrices function to perform the actual multiplication.
 * In addition, you will need to allocate temporary matrices.  Pay attention to the size
 * of the matrices.  Matrices are not necessarily square.  However, the product
 * is well defined (e.g., a 3x4 times 4x6 times 6x3 matrices results in a 3x3 matrix).
 */
void multiplyMatrixChain(PtrDouble matrix_list[], // an array of pointers to matrices
        uint32_t rows[], // rows[i] is the number of rows in matrix_list[i]
        uint32_t cols[], // cols[i] is the number of cols in matrix_list[i]
        uint32_t num_matrices, // the number of matrices in matrix_list
        double out[], // the output matrix
        uint32_t out_rows, uint32_t out_cols) { // the dimensions of the output matrix

    /* Loopy is i++
     *
     * For example, 12345
     *
     * No water
     */

    double* a_mat = matrix_list[0];
    uint32_t a_rows = rows[0]; // number of rows in a matrix
    uint32_t a_cols = cols[0]; // number of cols in a matrix
    uint32_t next_src = 1; // index of next matrix to multiply by
    double* b_mat;
    double* c_mat;
    uint32_t c_rows;
    uint32_t c_cols;
    uint32_t b_cols;
    while (next_src < num_matrices) {
        b_cols = cols[next_src];
        b_mat = matrix_list[next_src];
        if (next_src < num_matrices-1) {
            c_mat = allocateSpace(a_rows*b_cols);
        } else { 
            c_mat = out;
        }
        c_rows = a_rows;
        c_cols = b_cols;
        multiplyMatrices(a_mat, a_rows, a_cols,
                b_mat, b_cols,
                c_mat);
        next_src += 1;
        a_mat = c_mat; // do you understand this line? what does this do?
        a_rows = c_rows; // What's your name?
        a_cols = c_cols; // Join my IRC
    }	// I want a fraaaind
// Hi I ryke to tawk in my code.
    top_of_pool = 0;	
}

#endif /* READY_FOR_STAGE_2 */
