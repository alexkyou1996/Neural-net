#ifndef _NEURAL_LAYER_H_
#define _NEURAL_LAYER_H_

#include "neuron.h"
//! Structure to describe a single layer of a neural network
typedef struct  neural_layer_struct {
    //! pointer to array of pointers to neurons
    union {
        input_neuron_t **input_neurons;
        hidden_neuron_t **hidden_neurons;
        output_neuron_t **output_neurons;
    };
    //! type of layer
    int type;
    //! number of neurons in this layer
    uint32_t num_neurons;
} neural_layer_t;


//! Enum to describe type of layer in neural network
typedef enum layer_type_enum {
    LAYER_TYPE_INVALID = -1,
    LAYER_TYPE_INPUT,
    LAYER_TYPE_HIDDEN,
    LAYER_TYPE_OUTPUT,
} layer_type_t;

//! Function to create input layer object
/*
 * @params  uint32_t            Number of neurons to allocate for this layer
 *
 * @returns neural_layer_t *    The input layer object
 */
neural_layer_t *create_input_layer(uint32_t);

//! Function to set inputs to the input layer
/*
 * @params  neural_layer_t *    The input layer
 * @params  double *            The inputs
 * @params  uint32_t            The input size
 *
 * @returns bool                Whether success
 */
bool set_inputs_in_input_layer(neural_layer_t *, double *, uint32_t);

//! Function to create hidden layer object
/*
 * @params  uint32_t            Number of neurons to allocate for this layer
 *
 * @returns neural_layer_t *    The hidden layer object
 */
neural_layer_t *create_hidden_layer(uint32_t);

//! Function to create output layer object
/*
 * @params  uint32_t            Number of neurons to allocate for this layer
 *
 * @returns neural_layer_t *    The output layer object
 */
neural_layer_t *create_output_layer(uint32_t);

//! Function to destroy neural layer object
/*
 * @params  void *              The neural layer object to destroy
 */
void destroy_neural_layer(void *);

#endif
