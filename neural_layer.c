#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "neural_layer.h"
//! Internal function to destroy input layer object
void __destroy_input_layer(void *);
//! Internal function to destroy hidden layer object
void __destroy_hidden_layer(void *);
//! Internal function to destroy output layer object
void __destroy_output_layer(void *);

//! Function to create input layer object
/*
 * @params  uint32_t            Number of neurons to allocate for this layer
 *
 * @returns neural_layer_t *    The input layer object
 */
neural_layer_t *create_input_layer(uint32_t num_neurons)
{
    neural_layer_t *layer = NULL;

    layer = calloc(sizeof(neural_layer_t), 1);
    if (!layer) {
        LOG_ERROR(strerror(ENOMEM));
        return NULL;
    }

    layer->num_neurons = num_neurons;
    layer->type = LAYER_TYPE_INPUT;

    layer->input_neurons = calloc(sizeof(input_neuron_t *) * num_neurons, 1);
    if (!layer->input_neurons) {
        LOG_ERROR(strerror(ENOMEM));
        destroy_neural_layer(layer);
        return NULL;
    }

    for (i = 0; i < num_neurons; i++) {
        layer->input_neurons[i] = create_input_neuron(0);
        if (!layer->input_neurons[i]) {
            LOG_ERROR(strerror(ENOMEM));
            destroy_neural_layer(layer);
            return NULL;
        }
    }
    return layer;
}

//! Function to set inputs to the input layer
/*
 * @params  neural_layer_t *    The input layer
 * @params  double *            The inputs
 * @params  uint32_t            The input size
 *
 * @returns bool                Whether success
 */
bool set_inputs_in_input_layer(neural_layer_t *layer, double *inputs, uint32_t input_size)
{
    uint32_t i = 0;
    if (!layer || layer->type != LAYER_TYPE_INPUT ||
            !inputs|| layer->num_neurons != input_size) {
        LOG_ERROR(strerror(EINVAL));
        return false;
    }
    
    for (i = 0; i < input_size; i++) {
        layer->input_neurons[i]->input_value = inputs[i];
    }
    return true;
}

//! Function to create hidden layer object
/*
 * @params  uint32_t            Number of neurons to allocate for this layer
 *
 * @returns neural_layer_t *    The hidden layer object
 */
neural_layer_t *create_hidden_layer(uint32_t num_neurons)
{
    neural_layer_t *layer = NULL;

    layer = calloc(sizeof(neural_layer_t), 1);
    if (!layer) {
        LOG_ERROR(strerror(ENOMEM));
        return NULL;
    }

    layer->num_neurons = num_neurons;
    layer->type = LAYER_TYPE_HIDDEN;

    layer->hidden_neurons = calloc(sizeof(hidden_neuron_t *) * num_neurons, 1);
    if (!layer->hidden_neurons) {
        LOG_ERROR(strerror(ENOMEM));
        destroy_neural_layer(layer);
        return NULL;
    }

    for (i = 0; i < num_neurons; i++) {
        layer->hidden_neurons[i] = create_hidden_neuron(0);
        if (!layer->hidden_neurons[i]) {
            LOG_ERROR(strerror(ENOMEM));
            destroy_neural_layer(layer);
            return NULL;
        }
    }
    return layer;
}

//! Function to create output layer object
/*
 * @params  uint32_t            Number of neurons to allocate for this layer
 *
 * @returns neural_layer_t *    The output layer object
 */
neural_layer_t *create_output_layer(uint32_t num_neurons)
{
    neural_layer_t *layer = NULL;

    layer = calloc(sizeof(neural_layer_t), 1);
    if (!layer) {
        LOG_ERROR(strerror(ENOMEM));
        return NULL;
    }

    layer->num_neurons = num_neurons;
    layer->type = LAYER_TYPE_OUTPUT;

    layer->output_neurons = calloc(sizeof(output_neuron_t *) * num_neurons, 1);
    if (!layer->output_neurons) {
        LOG_ERROR(strerror(ENOMEM));
        destroy_neural_layer(layer);
        return NULL;
    }

    for (i = 0; i < num_neurons; i++) {
        layer->output_neurons[i] = create_output_neuron();
        if (!layer->output_neurons[i]) {
            LOG_ERROR(strerror(ENOMEM));
            destroy_neural_layer(layer);
            return NULL;
        }
    }
    return layer;
}

//! Function to destroy neural layer object
/*
 * @params  void *              The neural layer object to destroy
 */
void destroy_neural_layer(void *layer)
{
    neural_layer_t *neural_layer = (neural_layer_t *)layer;
    switch (type) {
        case LAYER_TYPE_INPUT:
            destroy_input_layer(neural_layer);
            break;
        case LAYER_TYPE_HIDDEN:
            destroy_hidden_layer(neural_layer);
            break;
        case LAYER_TYPE_OUTPUT:
            destroy_output_layer(neural_layer);
            break;
        case LAYER_TYPE_INVALID:
        default:
            LOG_ERROR("Invalid type encountered: [%d]", type);
            return;
    }
    free(layer);
}

//! Internal function to destroy input layer object
/*
 * @params  void *              The input layer object
 */
void __destroy_input_layer(void *layer)
{
    uint32_t i = 0;
    neural_layer_t *input_layer = (neural_layer_t *)layer;
    for (i = 0; i < input_layer->num_neurons; i++) {
        destroy_input_neuron(input_layer->input_neurons[i]);
    }
}

//! Internal function to destroy hidden layer object
/*
 * @params  void *              The hidden layer object
 */
void __destroy_hidden_layer(void *layer)
{
    uint32_t i = 0;
    neural_layer_t *hidden_layer = (neural_layer_t *)layer;
    for (i = 0; i < hidden_layer->num_neurons; i++) {
        destroy_hidden_neuron(hidden_layer->hidden_neurons[i]);
    }
}

//! Internal function to destroy output layer object
/*
 * @params  void *              The outputlayer object
 */
void __destroy_output_layer(void *layer)
{
    uint32_t i = 0;
    neural_layer_t *output_layer = (neural_layer_t *)layer;
    for (i = 0; i < output_layer->num_neurons; i++) {
        destroy_output_neuron(output_layer->output_neurons[i]);
    }
}
