#ifndef _NN_DATA_H_
#define _NN_DATA_H_

#include "matrix.h"

#define IMAGE_WIDTH  28
#define IMAGE_HEIGHT 28

typedef struct nn_data_struct {
    double pixels[IMAGE_WIDTH*IMAGE_HEIGHT];
    uint32_t label;
} nn_data_t;

typedef struct nn_data_batch_struct {
    uint32_t num_data;
    nn_data_t *data;
    int data_type;
} nn_data_batch_t;

typedef struct nn_data_suite_struct {
    uint32_t num_batch;
    nn_data_batch_t *batches;
} nn_data_suite_t;

typedef enum nn_data_type_enum {
    NN_DATA_TRAIN = 0,
    NN_DATA_TEST,
} nn_data_type_t;

//! Function to divide a batch of data into multiple batch, contained in a suite
/*
 * @params  nn_data_batch_t *   The batch to divide
 * @params  uint32_t            The number of data that should be held by each batch
 *
 * @returns nn_data_suite_t *   The suite containing the divided batch
 *
 * NOTE: The batch object passed as the parameter should not be destroyed while the suite
 *       is in use. Also, DO NOT modify the values of data referenced by the batches 
 *       inside the suite
 */
nn_data_suite_t *nn_divide_batch_into_suite(nn_data_batch_t *, uint32_t);

matrix_t *nn_data_to_matrix(nn_data_t *);
#endif
