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

//! Function copy another layer's structure without its values
/*
 * @params  neural_layer_t **   The source layers to copy
 * @params  uint32_t            The size of the layer
 *
 * @returns neural_layer_t **   The copied layers
 */
neural_layer_t **create_delta_layer(neural_layer_t **, uint32_t);

//! The sigmoid activation function for neuron firing
/*
 * @param double **     2D Matrix of neurons
 * @param size_t        Matrix row size
 * @param size_t        Matrix column size
 *
 * returns              2D Matrix of neurons with activation function applied
 */
double **sigmoid(double **, size_t, size_t);

//! Move to the next layer of neurons
/*
 * @param double **     2D Matrix of neurons
 * @param size_t        Matrix row size
 * @param size_t        Matrix column size
 *
 * returns              The resulting matrix
 */
double **feed_forward(double **, size_t, size_t);


//! Stochastic gradient descent
/*
 * @params  double ****   The training data
 * @params  int           The epoch
 * @params  int           The batch size
 * @params  double        ETA
 * @params  double ****   The test data
 */
void SGD(double ****, int, uint32_t, double, double ****);


//! Function to update training data
/*
 * @params  double ****   Training data
 * @params  double        ETA
 */
void update_mini_batches(double ****, double);

//! Function to find the difference between actual and desired
//! and change weights accordingly
/*
 * @params  double **     
 * @params  double **
 *
 * @returns double ****
 */
double ****back_prop(double **, double **);

//! Function to evaluate an input data and test the reuslt
/*
 * @params  nn_data_batch_t *   The testing data
 *
 * @returns bool                Whether success
 */
bool evaluate(nn_data_batch_t *);
#endif
