#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"
#include "matrix_list.h"
#include "logging.h"

//! Function to create an empty matrix list
/*
 * @returns matrix_list_t *     An empty matrix list
 */
matrix_list_t *mtxl_create_matrix_list()
{
    matrix_list_t *matrix_list = NULL;
    matrix_list = calloc(sizeof(matrix_list_t), 1);
    if (!matrix_list) {
        LOG_ERROR(strerror(ENOMEM));
        return NULL;
    }
    return matrix_list;
}

//! Function to append a matrix to the matrix list
/*
 * @params  matrix_list_t *     The matrix list
 * @params  matrix_t *          The matrix to append. Can be NULL
 *
 * @returns bool                Whether success
 */
bool mtxl_add_matrix(matrix_list_t *list, matrix_t *matrix)
{
    matrix_t **new_matrix_list = NULL;
    if (!list) {
        LOG_ERROR(strerror(EINVAL));
        return false;
    }
    new_matrix_list = realloc(list->matrix_list, sizeof(matrix_t *) * (list->num_matrix + 1));
    if (!new_matrix_list) {
        LOG_ERROR(strerror(ENOMEM));
        return false;
    }
    list->matrix_list = new_matrix_list;
    list->num_matrix++;
    list->matrix_list[list->num_matrix - 1] = matrix;
    return true;
}

bool mtxl_remove_matrix(matrix_list_t *list, uint32_t index)
{
    return true;
}


//! Function to destroy a matrix list
/*
 * @params  void *              The matrix list
 */
void mtxl_destroy_list(void *list)
{
    uint32_t i = 0;
    matrix_list_t *matrix_list = (matrix_list_t *)list;

    for (i = 0; i < matrix_list->num_matrix; i++) {
        mtx_destroy_matrix(matrix_list->matrix_list[i]);
    }
    free(matrix_list);
}
