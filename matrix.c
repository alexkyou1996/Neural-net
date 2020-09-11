#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "matrix.h"
#include "logging.h"
//! struct to describe matrix
typedef struct matrix_struct {
    // number of rows of the matrix
    uint32_t num_rows;
    // number of columns of the matrix
    uint32_t num_columns;
    // the cells
    double **cells;
} matrix_t;

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
matrix_t *mtx_create_matrix(uint32_t rows, uint32_t columns)
{
    matrix_t *matrix = NULL;
    uint32_t i = 0;
    if (!rows || !columns) {
        LOG_ERROR(strerror(EINVAL));
        return NULL;
    }
    matrix = calloc(sizeof(matrix_t), 1);
    if (!matrix) {
        LOG_ERROR(strerror(ENOMEM));
        return NULL;
    }
    matrix->cells = calloc(sizeof(double *), rows);
    if (!matrix->cells) {
        LOG_ERROR(strerror(ENOMEM));
        mtx_destroy_matrix(matrix);
        return NULL;
    }
    for (i = 0; i < rows; i++) {
        matrix->cells[i] = calloc(sizeof(double), columns);
        if (!matrix->cells[i]) {
            LOG_ERROR(strerror(ENOMEM));
            mtx_destroy_matrix(matrix);
            return NULL;
        }
    }
    return matrix;
}

//! Function to free the matrix object
/*
 * @params void *               The matrix object
 */
void mtx_destroy_matrix(void *matrix)
{
    uint32_t i = 0;
    matrix_t *m = (matrix_t *)matrix;
    for (i = 0; i < m->num_rows; i++) {
        free(m->cells[i]);
    }
    free(m->cells);
    free(m);
}

//! Function to perform matrix dot product operation
/*
 * @params  matrix_t *          The left operand
 * @params  matrix_t *          The right operand
 *
 * @returns matrix_t *          The product
 */
matrix_t *mtx_dot(matrix_t *matrix_left, matrix_t *matrix_right)
{
    matrix_t *product = NULL;
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t k = 0;
    if (!matrix_left || !matrix_right) {
        LOG_ERROR(strerror(EINVAL));
        return NULL;
    }
    if (matrix_left->num_columns != matrix_right->num_rows) {
        LOG_ERROR("Illegal matrix operation");
        return NULL;
    }
    product = mtx_create_matrix(matrix_left->num_rows, matrix_right->num_columns);
    if (!product) {
        LOG_ERROR("Failed to create matrix");
        return NULL;
    }
    for (i = 0; i < matrix_left->num_rows; i++) {
        for (j = 0; j < matrix_left->num_columns; j++) {
            for (k = 0; k < matrix_right->num_columns; k++) {
                product->cells[i][k] += 
                    matrix_left->cells[i][j] * matrix_right->cells[j][k];
            }
        }
    }
    return product;
}

//! Function to perform matrix addition
/*
 * @params  matrix_t *          The left operand
 * @params  matrix_t *          The right operand
 *
 * @returns matrix_t *          The result
 */
matrix_t *mtx_add(matrix_t *matrix_left, matrix_t *matrix_right)
{
    matrix_t *sum = NULL;
    uint32_t i = 0;
    uint32_t j = 0;
    if (!matrix_left || !matrix_right) {
        LOG_ERROR(strerror(EINVAL));
        return NULL;
    }
    if (matrix_left->num_rows != matrix_right->num_rows ||
            matrix_left->num_columns != matrix_right->num_columns) {
        LOG_ERROR("Illegal matrix operation");
        return NULL;
    }
    sum = mtx_create_matrix(matrix_left->num_rows, matrix_left->num_columns);
    if (!sum) {
        LOG_ERROR("Failed to create matrix");
        return NULL;
    }
    for (i = 0; i < matrix_left->num_rows; i++) {
        for (j = 0; j < matrix_left->num_columns; j++) {
            sum->cells[i][j] = matrix_left->cells[i][j] + matrix_right->cells[i][j];
        }
    }
    return sum;
}


//! Function to perform matrix subtraction
/*
 * @params  matrix_t *          The left operand
 * @params  matrix_t *          The right operand
 *
 * @returns matrix_t *          The result
 */
matrix_t *mtx_subtract(matrix_t *matrix_left, matrix_t *matrix_right)
{
    matrix_t *sum = NULL;
    uint32_t i = 0;
    uint32_t j = 0;
    if (!matrix_left || !matrix_right) {
        LOG_ERROR(strerror(EINVAL));
        return NULL;
    }
    if (matrix_left->num_rows != matrix_right->num_rows ||
            matrix_left->num_columns != matrix_right->num_columns) {
        LOG_ERROR("Illegal matrix operation");
        return NULL;
    }
    sum = mtx_create_matrix(matrix_left->num_rows, matrix_left->num_columns);
    if (!sum) {
        LOG_ERROR("Failed to create matrix");
        return NULL;
    }
    for (i = 0; i < matrix_left->num_rows; i++) {
        for (j = 0; j < matrix_left->num_columns; j++) {
            sum->cells[i][j] = matrix_left->cells[i][j] - matrix_right->cells[i][j];
        }
    }
    return sum;
}

//! Function to transpose a matrix
/*
 * @params  matrix_t *          The matrix to transpose
 *
 * @returns matrix_t *          The tranposed matrix
 */
matrix_t *mtx_transpose(matrix_t *matrix)
{
    matrix_t *transposed_matrix = NULL;
    uint32_t i = 0;
    uint32_t j = 0;
    if (!matrix) {
        LOG_ERROR(strerror(EINVAL));
        return NULL;
    }
    transposed_matrix = mtx_create_matrix(matrix->num_columns, matrix->num_rows);
    if (!transposed_matrix) { 
        LOG_ERROR("Failed to create a matrix");
        return NULL;
    }
    for (i = 0; i < matrix->num_rows; i++) {
        for (j = 0; j < matrix->num_columns; j++) {
            transposed_matrix->cells[j][i] = matrix->cells[i][j];
        }
    }
    return transposed_matrix;
}

//! Function to retrieve a value at specific index of a matrix
/*
 * @params  matrix_t *          The matrix
 * @params  uint32_t            The row index
 * @params  uint32_t            The column index
 * @params  double *            The buffer to store the value
 *
 * @returns bool                Whether success
 */
bool mtx_at(matrix_t *matrix, uint32_t row, uint32_t column, double *value)
{
    if (!matrix || !value) {
        LOG_ERROR(strerror(EINVAL));
        return false;
    }
    if (row >= matrix->num_rows || column >= matrix->num_columns) {
        LOG_ERROR("Index out of bounds");
        return false;
    }
    *value = matrix->cells[row][column];
    return true;
}

//! Function to set a value at specific index of a matrix
/*
 * @params  matrix_t *          The matrix
 * @params  uint32_t            The row index
 * @params  uint32_t            The column index
 * @params  double *            The value
 *
 * @returns bool                Whether success
 */
bool mtx_set_cell(matrix_t *matrix, uint32_t row, uint32_t column, double value)
{
    if (!matrix) {
        LOG_ERROR(strerror(EINVAL));
        return false;
    }
    if (row >= matrix->num_rows || column >= matrix->num_columns) {
        LOG_ERROR("Index out of bounds");
        return false;
    }
    matrix->cells[row][column] = value;
    return true;
}
