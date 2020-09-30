#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "nn_data.h"
#include "neural_layer.h"
typedef struct network_struct network_t;
//! Function to create and initialize a neural network object
/*
 * @params  uint32_t *          Number of neurons to allocate at each layer
 * @params  uint32_t            Number of layers to allocate
 *
 * @returns network_t           The neural network object
 */
network_t *create_network(uint32_t *, uint32_t);

//! Function to train the neural net
/*
 * @params  network_t *         The neural network
 * @params  int                 Number of training sessions
 * @params  uint32_t            Number of tests per session
 * @params  double              The effect of each session
 * @params  nn_data_batch_t *   The test batch
 */
bool train(network_t *, nn_data_batch_t *, int, uint32_t, double, nn_data_batch_t *);

//! Function to evaluate an input data and test the reuslt
/*
 * @params  nn_data_batch_t *   The testing data
 *
 * @returns bool                Whether success
 */
bool evaluate(nn_data_batch_t *);
#endif
