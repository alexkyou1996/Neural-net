#include <stdbool.h>
#include <stdint.h>

#include "matrix.h"

//! Structure to desscribe the matrix list
typedef struct matrix_list_struct {
    // the matrix list
    matrix_t **matrix_list;
    // number of matrix
    uint32_t num_matrix;
} matrix_list_t;

//! Function to create an empty matrix list
/*
 * @returns matrix_list_t *     An empty matrix list
 */
matrix_list_t *mtxl_create_matrix_list();

//! Function to append a matrix to the matrix list
/*
 * @params  matrix_list_t *     The matrix list
 * @params  matrix_t *          The matrix to append
 *
 * @returns bool                Whether success
 */
bool mtxl_add_matrix(matrix_list_t *, matrix_t *);

//! Function to destroy a matrix list
/*
 * @params  void *              The matrix list
 */
void mtxl_destroy_list(void *);

