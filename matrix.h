#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdbool.h>
#include <stdint.h>

//! Forward declartion for the matrix object
typedef struct matrix_struct matrix_t;

//! Function to create the matrix object
/*
 * @params  uint32_t            Number of rows
 * @params  uint32_t            Number of columns
 *
 * returns  matrix_t *          The matrix object
 *
 *  NOTE: I can probably implement this in single array and
 *        create more efficient algorithm. Check if this bottlenecks later
 */
matrix_t *mtx_create_matrix(uint32_t, uint32_t);

//! Function to free the matrix object
/*
 * @params void *               The matrix object
 */
void mtx_destroy_matrix(void *);

//! Function to perform matrix dot product operation
/*
 * @params  matrix_t *          The left operand
 * @params  matrix_t *          The right operand
 *
 * @returns matrix_t *          The product
 */
matrix_t *mtx_dot(matrix_t *, matrix_t *);

//! Function to perform matrix addition
/*
 * @params  matrix_t *          The left operand
 * @params  matrix_t *          The right operand
 *
 * @returns matrix_t *          The result
 */
matrix_t *mtx_add(matrix_t *, matrix_t *);

//! Function to perform matrix subtraction
/*
 * @params  matrix_t *          The left operand
 * @params  matrix_t *          The right operand
 *
 * @returns matrix_t *          The result
 */
matrix_t *mtx_subtract(matrix_t *, matrix_t *);

//! Function to transpose a matrix
/*
 * @params  matrix_t *          The matrix to transpose
 *
 * @returns matrix_t *          The tranposed matrix
 */
matrix_t *mtx_transpose(matrix_t *);

//! Function to multiply all the members in a matrix by a value
/*
 * @params  matrix_t *          The matrix
 * @params  double              The value to multiply by
 *
 * @returns matrix_t *          The result matrix
 */
matrix_t *mtx_multiply_by_single_value(matrix_t *, double);

//! Function to multiply specific columns from two matrix
/*
 * @params  matrix_t *          LHS Matrix
 * @params  uint23_t            The column index to multiply
 * @params  matrix_t *          RHS Matrix
 * @params  uint23_t            The column index to multiply
 *
 * @returns matrix_t *          The resulting matrix
 */
matrix_t *mtx_multiply_column_vectors(matrix_t *, uint32_t, matrix_t *, uint32_t);

//! Function to retrieve a value at specific index of a matrix
/*
 * @params  matrix_t *          The matrix
 * @params  uint32_t            The row index
 * @params  uint32_t            The column index
 * @params  double *            The buffer to store the value
 *
 * @returns bool                Whether success
 */
bool mtx_at(matrix_t *, uint32_t, uint32_t, double *);

//! Function to set a value at specific index of a matrix
/*
 * @params  matrix_t *          The matrix
 * @params  uint32_t            The row index
 * @params  uint32_t            The column index
 * @params  double *            The value
 *
 * @returns bool                Whether success
 */
bool mtx_set_cell(matrix_t *, uint32_t, uint32_t, double);

//! Function to retrieve the number of rows in a matrix
/*
 * @params  matrix_t *          The matrix
 *
 * @returns uint32_t            The number of rows
 */
uint32_t mtx_get_num_rows(matrix_t *);

//! Function to set a sepcific row with array of values
/*
 * @params  matrix_t *          The matrix
 * @params  uint32_t            Row index
 * @params  double *            Array of values
 * @params  uint32_t            size of array
 */
bool mtx_set_row(matrix_t *, uint32_t, double *, uint32_t);

//! Function to set a sepcific columnwith array of values
/*
 * @params  matrix_t *          The matrix
 * @params  uint32_t            column index
 * @params  double *            Array of values
 * @params  uint32_t            size of array
 */
bool mtx_set_column(matrix_t *, uint32_t, double *, uint32_t);

//! Function to retrieve the number of columns in a matrix
/*
 * @params  matrix_t *          The matrix
 *
 * @returns uint32_t            The number of columns
 */
uint32_t mtx_get_num_columns(matrix_t *);

//! DEBUG function to print a matrix in human readable format
/*
 * @params  matrix_t *          The matrix to print
 */
void mtx_print(matrix_t *);

#endif
