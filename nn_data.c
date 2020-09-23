#include "nn_data.h"

nn_data_batch_t *nn_create_data_batch(uint32_t num_data, int data_type)
{
    nn_data_batch_t *batch = NULL;
    if (!num_data) {
        LOG_ERROR(strerror(EINVAL));
        return NULL;
    }
    batch = calloc(sizeof(nn_data_batch_t), 1);
    if (!batch) {
        LOG_ERROR(strerror(ENOMEM));
        return NULL;
    }
    batch->data = calloc(sizeof(nn_data_batch_t), num_data);
    if (!batch->data) {
        LOG_ERROR(strerror(ENOMEM));
        destroy_data_batch(batch);
        return NULL;
    }
    batch->data_type = data_type;
    return batch;
}

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
nn_data_suite_t *nn_divide_batch_into_suite(nn_data_batch_t *batch, uint32_t num_data_per_batch)
{
    nn_data_suite_t *suite = NULL;
    nn_data_batch_t *batch = NULL;
    nn_data_t *data = NULL;
    uint32_t num_batches = 0;
    if (!batch|| !num_data_per_batch) {
        LOG_ERROR(strerror(EINVAL));
        return NULL;
    }
    if (num_data_per_batch > batch->num_data) {
        LOG_ERROR("Batch size is not big enough");
        return NULL;
    }
    num_batches = batch->num_data / num_data_per_batch;
    // create suite first
    suite = calloc(sizeof(nn_data_suite_t), 1);
    if (!suite)  {
        LOG_ERROR(strerror(ENOMEM));
        return NULL;
    }
    // create array of pointers to each batch
    suite->batches = calloc(sizeof(nn_data_batch_t), num_batches);
    if (!suite->batches) {
        LOG_ERROR(strerror(ENOMEM));
        destroy_data_suite(suite);
        return NULL;
    }
    suite->num_batch = num_batches;
    for (i = 0; i < num_batches; i++) {
        suite->batches[i].num_data = num_data_per_batch;
        suite->batches[i].data_type = batch->data_type;
        suite->batches[i].data = &(batch->data[i * num_data_per_batch]);
    }
    return suite;
}

//! Function to create a matrix representation of the data object
/*
 * @params  nn_data_t *         The data object
 *
 * @returns matrix_t *          The matrix
 */
matrix_t *nn_data_to_matrix(nn_data_t *data)
{
    uint32_t i = 0;
    matrix_t *matrix = NULL;
    if (!data) {
        LOG_ERROR(strerror(EINVAL));
        return NULL;
    }
    matrix = mtx_create_matrix(IMAGE_WIDTH * IMAGE_HEIGHT, 1);
    if (!matrix) {
        LOG_ERROR("Failed to create matrix");
        return NULL;
    }
    if (!mtx_set_column(matrix, 0, &data->pixels[0], IMAGE_WIDTH * IMAGE_HEIGHT)) {
        LOG_ERROR("Failed to fill in the matrix");
        mtx_destroy_matrix(matrix);
        return NULL;
    }
    return matrix;
}

/*
nn_data_suite_t *divide_batch_into_suite(nn_data_batch_t *batch, uint32_t num_data_per_batch)
{
    nn_data_suite_t *suite = NULL;
    nn_data_batch_t *batch = NULL;
    nn_data_t *data = NULL;
    uint32_t num_batches = 0;
    if (!batch|| !num_data_per_batch) {
        LOG_ERROR(strerror(EINVAL));
        return NULL;
    }
    if (num_data_per_batch > batch->num_data) {
        LOG_ERROR("Batch size is not big enough");
        return NULL;
    }
    num_batches = batch->num_data / num_data_per_batch;
    // create suite first
    suite = calloc(sizeof(nn_data_batch_t), 1);
    if (!suite)  {
        LOG_ERROR(strerror(ENOMEM));
        return NULL;
    }
    // create array of batch
    suite->nn_data_batch = calloc(sizeof(nn_data_batch_t), num_batches);
    if (!suite->nn_data_batch) {
        LOG_ERROR(strerror(EINVAL));
        destroy_data_suite(suite);
        return NULL;
    }
    suite->num_batch = num_batches;
    // creat array of data for each batch in suite
    for (i = 0; i < num_batches; i++) {
        data = calloc(sizeof(nn_data_t), num_data_per_batch);
        if (!data) {
            LOG_ERROR(strerror(ENOMEM));
            destroy_data_suite(suite);
            return NULL;
        }
        suite->nn_data_batch[i].data = data;
        suite->nn_data_batch[i].num_data = num_data_per_batch;
        suite->nn_data_batch[i].data_type = batch->data_type;
    }
}
*/
void destroy_data_batch(void *data_batch)
{ 
    nn_data_batch_t *batch = (nn_data_batch_t *)data_batch;
    if (data_batch->data) {
        free(data_batch->data);
    }
    free(data_batch);
}

void destroy_data_suite(void *data_suite)
{
}
