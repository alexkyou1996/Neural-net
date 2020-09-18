#include <stdbool.h>
#include <stdlib.h>

#include "neuron.h"
#define NUM_LAYERS 3

#define NUM_INPUT_NEURONS 784
#define NUM_MIDDLE_NEURONS 30
#define NUM_OUTPUT_NEURONS 10
#define MIN_NEURAL_LAYER 3

#define INPUT_LAYER_INDEX 0
#define HIDDEN_LAYER_INDEX 1

//! Function to create layers within neural net with zeroed bias and weights
neural_layer_t **__create_layers(uint32_t *, uint32_t);
//! Internal function to initialize input layer within the neural network
bool __initialize_input_layer(neural_layer_t **, uint32_t *);
//! Internal function to initialize hidden layer within the neural network
bool __initialize_hidden_layer(neural_layer_t **, uint32_t *, uint32_t);
//! Internal function to initialize output layer within the neural network
bool __initialize_output_layer(neural_layer_t **, uint32_t *, uint32_t);
//! Internal function to initialize bias and weights in the neural network
void __initialize_bias_and_weights(network_t *);
//! Internal function to initialize bias of the input layer
void __init_input_bias(neural_layer_t **);
//! Internal function to initialize bias of the hidden layers
void __init_hidden_bias(neural_layer_t **, uint32_t);
//! Internal function to initialize bias of the output layers
void __init_output_bias(neural_layer_t **, uint32_t);
//! Internal function to initialize weights of the input layer
void __init_input_weights(neural_layer_t **);
//! Internal function to initialize weigths of the hidden layers
void __init_hidden_weights(neural_layer_t **, uint32_t);
//! Internal function to initialize weigths of the output layers
void __init_output_weights(neural_layer_t **, uint32_t);
//! Internal function to generate random double value
double __gen_random_double(double, double);
neural_layer_t *__get_input_layer(network_t *);
neural_layer_t *__get_hidden_layer(network_t *, uint32_t);
neural_layer_t *__get_ouput_layer(network_t *);


//! Structure to describe the neural network object
typedef struct network_struct {
    //! number of layers in the neural network
    size_t num_layers;
    //! reference to the layers
    neural_layer_t **layers;
} network_t;


//! Function to create and initialize a neural network object
/*
 * @params  uint32_t *          Number of neurons to allocate at each layer
 * @params  uint32_t            Number of layers to allocate
 *
 * @returns network_t           The neural network object
 */
network_t create_network(uint32_t *num_neurons_per_layer, uint32_t num_layers)
{
    neural_layer_t **layers = NULL;
    network_t *network = NULL;
    uint32_t i = 0;
    if (!num_neurons_per_layer || !num_layers || num_layer < MIN_NEURAL_LAYER) {
        log_error(strerror(EINVAL));
        return NULL;
    }

    network = calloc(sizeof(network_t), 1);
    if (!network) {
        log_error(strerror(ENOMEM));
        return NULL;
    }
    layers = __create_layers(num_neurons_per_layer, num_layers);
    if (!layers) {
        log_error("Failed to create layers");
        free(network);
        return NULL;
    }
    network->num_layers = num_layers;
    network->layers = layers;
    __initialize_bias_and_weights(network);
    return network;
}

//! Function to feed input data to the neural network
/* 
 * @params  network_t *         The neural network
 * @params  nn_data_t *         The data object to feed in
 *
 * @returns bool                Whether success
 */
bool feed_input_to_network(network_t *network, nn_data_t *input_data)
{
    neural_layer *input_layer = NULL;
    double *serialized_inputs = NULL;
    uint32_t i = 0;
    if (!network || !input) {
        log_error(strerror(EINVAL));
        return false;
    }
    input_layer = __get_input_layer(network);
    serialized_inputs = nn_data_serialize_input(input_data);
    set_inputs_in_input_layer(input_layer, serialized_inputs);
    return true;
}

//! Function copy another layer's structure without its values
/*
 * @params  neural_layer_t **   The source layers to copy
 * @params  uint32_t            The size of the layer
 *
 * @returns neural_layer_t **   The copied layers
 */
neural_layer_t **create_delta_layer(neural_layer_t **layers_to_copy, uint32_t num_layers)
{
    uint32_t *num_neurons_per_layer = 0;
    uint32_t i = 0;
    neural_layer_t **layers = NULL;

    num_neurons_per_layer = calloc(sizeof(uint32_t) * num_layers, 1);
    if (!num_neurons_per_layer) {
        log_error(strerror(ENOMEM));
        return NULL;
    }
    for (i = 0; i < num_layers; i++) {
        num_neurons_per_layer[i] = layers_to_copy[i]->num_neurons;
    }
    layers = __create_layers(num_neurons_per_layer, num_layers);
    if (!layers) {
        log_error("Failed to create layers");
        free(num_neurons_per_layer);
        return NULL;
    }
    free(num_neurons_per_layer);
    return layers;
}

//! Function to create layers within neural net with zeroed bias and weights
/*
 * @params  uint32_t *          Number of neurons to allocate at each layer
 * @params  uint32_t            Number of layers to allocate
 *
 * @returns neural_layer_t **   The neural layer object
 */
neural_layer_t **__create_layers(uint32_t *num_neurons_per_layer, uint32_t num_layers)
{
    neural_layer_t **layers = NULL;
    layers = calloc(sizeof(neural_layer_t *), num_layers);
    if (!layers) {
        log_error(strerror(ENOMEM));
        return NULL;
    }
    if (!__initialize_input_layer(layers, num_neurons_per_layer)) {
        log_error(strerror("Failed to initialize input layer"));
        return false;
    }
    if (!__initialize_hidden_layer(layers, num_neurons_per_layer, num_layers)) {
        log_error(strerror("Failed to initialize hidden layer"));
        return false;
    }
    if (!__initialize_output_layer(layers, num_neurons_per_layer, num_layers)) {
        log_error(strerror("Failed to initialize output layer"));
        return false;
    }
    return layers;
}

//! Internal function to initialize input layer within the neural network
/*
 * @params  nerual_layer_t **   The layers of neural network
 * @params  uint32_t *          The number of neurons to create for at each layer
 *
 * @returns bool                Whether successful
 */
bool __initialize_input_layer(neural_layer_t **layers, uint32_t *num_neurons_per_layer) 
{
    neural_layer_t *layer = NULL;
    layer = create_input_layer(num_neurons[0]);
    if (!layer) {
        log_error(strerror(ENOMEM));
        return false;
    }
    layers[0] = layer;
    return true;
}

//! Internal function to initialize hidden layer within the neural network
/*
 * @params  nerual_layer_t **   The layers of neural network
 * @params  uint32_t *          The number of neurons to create for at each layer
 * @params  uint32_t            Number of layers in the neural network
 *
 * @returns bool                Whether successful
 */
bool __initialize_hidden_layer(neural_layer_t **layers, 
        uint32_t *num_neurons_per_layer, uint32_t num_layers) 
{
    uint32_t i = 0;
    neural_layer_t *layer = NULL;
    for (i = 1; i < num_layers - 1; i++) {
        layer = create_hidden_layer(num_neurons_per_layer[i]);
        if (!layer) {
            log_error(sterror(ENOMEM));
            return false;
        }
        layers[i] = layer;
    }
    return true;
}

//! Internal function to initialize output layer within the neural network
/*
 * @params  nerual_layer_t **   The layers of neural network
 * @params  uint32_t *          The number of neurons to create for at each layer
 * @params  uint32_t            Number of layers in the neural network
 *
 * @returns bool                Whether successful
 */
bool __initialize_output_layer(neural_layer_t **layers,
        uint32_t *num_neurons_per_layer, uint32_t num_layers)
{
    layer = create_output_layer(num_neurons_per_layer[num_layers - 1]);
    if (!layer) {
        log_error(strerror(ENOMEM));
        return false;
    }
    layers[num_layer - 1] = layer;
    return true;
}

//! Internal function to initialize bias and weights in the neural network
/*
 * @params  network_t *         The neural network object
 */
void __initialize_bias_and_weights(network_t *network)
{
    srand(time(NULL));
    __init_input_bias(network->layers);
    __init_hidden_bias(network->layers, network->num_layers);
    __init_output_bias(network->layers, network->num_layers);
    __init_input_weights(network->layers);
    __init_hidden_weights(network->layers, network->num_layers);
    __init_output_weights(network->layers, network->num_layers);
}

//! Internal function to initialize bias of the input layer
/*
 * @params neural_layer_t **    The layers in the neural network
 */
void __init_input_bias(neural_layer_t **layers)
{
    neural_layer_t *input_layer = NULL;
    input_layer = layer[0];
    return;
} 

//! Internal function to initialize bias of the hidden layers
/*
 * @params neural_layer_t **    The layers in the neural network
 * @params uint32_t             The total number of layers in NN
 */
void __init_hidden_bias(neural_layer_t **layers, uint32_t num_layers)
{
    neural_layer_t *hidden_layer = NULL;
    double bias = 0;
    uint32_t i = 0;
    uint32_t j = 0;

    for (i = 1; i < num_layers - 1; i++) {
        hidden_layer = layers[i];
        for (j = 0; j < hidden_layer->num_neurons; j++) {
            bias = __gen_random_double(0.0, 1.0) * 4 - 2;
            hidden_layer->hidden_neurons[j]->bias = bias;
        }
    }
}

//! Internal function to initialize bias of the output layers
/*
 * @params neural_layer_t **    The layers in the neural network
 * @params uint32_t             The total number of layers in NN
 */
void __init_output_bias(neural_layer_t **layers, uint32_t num_layers)
{
    neural_layer_t *output_layer = NULL;
    double_bias = 0;
    uint32_t i = 0;
    
    output_layer = layers[num_layers - 1];
    for (i = 0; i < output_layer->num_neurons; i++) {
        bias = __gen_random_double(0.0, 1.0) * 4 - 2;
        output_layer->output_neurons[i]->bias = bias;
    }
}

//! Internal function to initialize weights of the input layer
/*
 * @params neural_layer_t **    The layers in the neural network
 */
void __init_input_weights(neural_layer_t **layers)
{
    const nueral_layer_t *next_hidden_layer = NULL;
    neural_layer_t *input_layer = NULL; 
    double *weights_array = NULL;
    uint32_t num_weights = 0;
    double weight = 0.0f;
    uint32_t i = 0;
    uint32_t j = 0;

    input_layer = layers[0];
    next_hidden_layer = layers[1];
    num_weights = next_hidden_layer->num_neurons;
    // each neurons in the input layer propagates an output to all thew neurons in the next layer
    for (i = 0; i < input_layer->num_neurons; i++) {
        weights_array = calloc(sizeof(double) * num_weights, 1);
        if (!weights_array) {
            log_error(strerror(ENOMEM));
            return;
        }
        input_layer->input_neurons[i]->weights = weights_array;
        input_layer->input_neurons[i]->num_weights = num_weights;
        for (j = 0; j < num_weights; j++) {
            weight = __gen_random_double(0.0, 1.0) * 4 - 2;
            input_layer->input_neurons[i]->weights[j] = weight;
        }
    }
}

//! Internal function to initialize weigths of the hidden layers
/*
 * @params neural_layer_t **    The layers in the neural network
 * @params uint32_t             The total number of layers in NN
 */
void __init_hidden_weights(neural_layer_t **layers, uint32_t num_layers)
{
    const nueral_layer_t *next_layer = NULL;
    neural_layer_t *hidden_layer = NULL; 
    double *weights_array = NULL;
    uint32_t num_weights = 0;
    double weight = 0.0f;
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t k = 0;

    // hidden layer start at index 1
    hidden_layer = layers[HIDDEN_LAYER_INDEX];
    next_layer = layers[HIDDEN_LAYER_INDEX + 1];
    for (i = 1; i < num_layers - 1; i++) {
        hidden_layer = layers[i];
        next_layer = layers[i + 1];
        num_weights = next_layer->num_neurons;
        for (j = 0; j < hidden_layer->num_neurons; j++) {
            weights_array = calloc(sizeof(double) * num_weights, 1);
            if (!weights_array) {
                log_error(strerror(ENOMEM));
                return;
            }
            hidden_layer->hidden_neurons[j]->weights = weights_array;
            hidden_layer->hidden_neurons[j]->num_weights = num_weights;
            for (k = 0; k < num_weights; k++) {
                weight = __gen_random_double(0.0, 1.0) * 4 - 2;
                hidden_layer->input_neurons[j]->weights[k] = weight;
            }
        }
    }
}

//! Internal function to retrieve the input layer
/*
 * @params  network_t *         The neural network
 *
 * @returns neural_layer_t *    The input layer
 */
neural_layer_t *__get_input_layer(network_t *network)
{
    neural_layer_t *layer = NULL;
    if (!network || !network->num_layers || !network->layers) {
        log_error(strerror(EINVAL));
        return NULL;
    }
    layer = network->layers[0];
    return (layer->type == LAYER_TYPE_INPUT) ? layer : NULL;
}
//! Internal function to retrieve a specific hidden layer via index
/*
 * @params  network_t *         The neural network
 * @params  uint32_t            The index of the hidden layer to retreive
 *
 * @returns neural_layer_t *    The hidden layer
 */
neural_layer_t *__get_hidden_layer(network_t *network, uint32_t index)
{
    neural_layer_t *layer = NULL;
    if (!network || !network->num_layers || network->layers) {
        log_error(strerror(EINVAL));
        return NULL;
    }
    if ((network->num_layers - 2) <= index) {
        log_error(strerror(EINVAL));
        return NULL;
    }
    layer = network->layers[index + 1];
    return (layer->type == LAYER_TYPE_HIDDEN) ? layer : NULL;
}
//! Internal function to retrieve the output layer
/*
 * @params  network_t *         The neural network
 *
 * @returns neural_layer_t *    The output layer
 */
neural_layer_t *__get_ouput_layer(network_t *network)
{
    neural_layer_t *layer = NULL;
    if (!network || !network->num_layers || network->layers) {
        log_error(strerror(EINVAL));
        return NULL;
    }
    layer = network->layers[network->num_layers - 1];
    return (layer->type == LAYER_TYPE_OUTPUT) ? layer : NULL;
}

bool train(network_t *network, nn_data_batch *training_data, int epochs, uint32_t num_test_per_batch, double eta, nn_data_t *test_data)
{
    nn_data_batch_t *batch = NULL;
    nn_data_suite *suite = NULL;
    uint32_t i = 0;
    if (!network|| !trainging_data || !test_data) {
        log_error(strerror(EINVAL));
        return false;
    }
    for (i = 0; i < epochs; i++) {
        suite = divide_batch_into_suite(training_data, num_test_per_batch);
        if (!suite) {
            log_error("Failed to create divide the training batch");
            return false;
        }
        if (!update_suite(network, suite, eta)) {
            log_error("Failed to update the mini batch");
            return false;
        }
        if (!evaluate(test_data)) {
            log_error("Failed to evalute test data");
            return false;
        }
    }
    return true;
}

bool update_suite(network_t *network, nn_suite_t *training_suite, double learning_rate)
{
    uint32_t serialized_expected_output_size = 0;
    double *serizlied_expected_output = NULL;
    uint32_t serialized_input_size = 0;
    double *serialized_input = NULL;
    uint32_t i = 0;
    neural_layer_t **delta_layers = NULL;
    delta_layers = create_delta_layer(network->layers, network->num_layers);
    if (!layers) {
        log_error("Failed to create delta layers");
        return NULL;
    }
    for (i = 0; i < training_suite->num_batch; i++) {
        __backprop(network, training_suite->batches[i]);
    }
}

void *__backprop_training_batch(network_t *network, nn_data_batch_t *training_batch)
{
    neural_layer_t *delta_layers = NULL;
    uint32_t i = 0;
    delta_layers = create_delta_layer(network->layers, network->num_layers);
    if (!delta_layers) {
        log_error("Failed to create delta layer");
        return NULL;
    }

    for (i = 0; i < training_batch->num_data; i++) {

    }

    // b = 
    // actviation = input data
    // activation = x[768][1]

    // activations = bias layer

    // prepend input data(first layer) to activations (bias layer). Before this activations only had bias for 2nd layer and on

    // zs = clone of bias structure

}

bool __backprop_training_data(network_t *network, nn_data_t *training_data)
{
    matrix_t *result_dot = NULL;
    matrix_t *result_sum = NULL;
    matrix_t **results = NULL;
    uint32_t i = 0;
    for (i = 0; i < network->num_layers - 1; i++) {
        // derive weight_matrix
        result_dot = mtx_dot(weight_matrix, input_matrix);
        if (!result_dot) {
            return false;
        }
        result_sum = mtx_sum(result_dot, bias_matrix);
        if (!result_sum) {
            return false;
        }
    }
}

//! Internal function to initialize weigths of the output layers
/*
 * @params neural_layer_t **    The layers in the neural network
 * @params uint32_t             The total number of layers in NN
 */
void __init_output_weights(neural_layer_t **layers, uint32_t num_layers)
{
    neural_layer_t *output_layer = layers[num_layers - 1];
    return;
}

//! Internal function to generate random double value
/*
 * @params  double              The minimum value
 * @params  double              The maximum value
 *
 * @returns double              The random value
 */
double __gen_random_double(double min, double max)
{
    double range = max - min;
    double div = RAND_MAX / range;
    return min + (rand() / div);
}
