#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <math.h>

#include "logging.h"
#include "matrix_list.h"
#include "network.h"
#include "matrix.h"
#include "neural_layer.h"
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
neural_layer_t *__get_layer_by_index(network_t *, uint32_t);
bool __create_matrix_list_of_bias_and_weights(network_t *, matrix_list_t **, matrix_list_t **, bool);
bool __backprop_training_data(network_t *, nn_data_t *, matrix_list_t **, matrix_list_t **);
// TODO: implement
bool __update_bias_and_weights(network_t *, matrix_list_t *, matrix_list_t *);
bool __feed_forward_for_backprop(network_t *, nn_data_t *, matrix_list_t **, matrix_list_t **);
bool __backprop_outputs_and_activations(network_t *, nn_data_t *,
        matrix_list_t *, matrix_list_t *, matrix_list_t ** , matrix_list_t **);
matrix_t *__create_weight_matrix(neural_layer_t *, bool);
matrix_t *__create_bias_matrix(neural_layer_t *, bool);
bool backprop(network_t *, nn_data_suite_t *, double);
bool __backprop_training_batch(network_t *, nn_data_batch_t *, double);
matrix_t *__create_label_matrix(uint32_t, uint32_t);
matrix_t *__apply_sigmoid(matrix_t *);
matrix_t *__apply_sigmoid_prime(matrix_t *);
matrix_t *__apply_cost_function(matrix_t *, uint32_t);

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
network_t *create_network(uint32_t *num_neurons_per_layer, uint32_t num_layers)
{
    neural_layer_t **layers = NULL;
    network_t *network = NULL;
    uint32_t i = 0;
    if (!num_neurons_per_layer || !num_layers || num_layers < MIN_NEURAL_LAYER) {
        LOG_ERROR(strerror(EINVAL));
        return NULL;
    }

    network = calloc(sizeof(network_t), 1);
    if (!network) {
        LOG_ERROR(strerror(ENOMEM));
        return NULL;
    }
    layers = __create_layers(num_neurons_per_layer, num_layers);
    if (!layers) {
        LOG_ERROR("Failed to create layers");
        free(network);
        return NULL;
    }
    network->num_layers = num_layers;
    network->layers = layers;
    __initialize_bias_and_weights(network);
    return network;
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
        LOG_ERROR(strerror(ENOMEM));
        return NULL;
    }
    for (i = 0; i < num_layers; i++) {
        num_neurons_per_layer[i] = layers_to_copy[i]->num_neurons;
    }
    layers = __create_layers(num_neurons_per_layer, num_layers);
    if (!layers) {
        LOG_ERROR("Failed to create layers");
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
        LOG_ERROR(strerror(ENOMEM));
        return NULL;
    }
    if (!__initialize_input_layer(layers, num_neurons_per_layer)) {
        LOG_ERROR("Failed to initialize input layer");
        return false;
    }
    if (!__initialize_hidden_layer(layers, num_neurons_per_layer, num_layers)) {
        LOG_ERROR("Failed to initialize hidden layer");
        return false;
    }
    if (!__initialize_output_layer(layers, num_neurons_per_layer, num_layers)) {
        LOG_ERROR("Failed to initialize output layer");
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
    layer = create_input_layer(num_neurons_per_layer[0]);
    if (!layer) {
        LOG_ERROR(strerror(ENOMEM));
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
            LOG_ERROR(strerror(ENOMEM));
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
    neural_layer_t *layer = NULL;
    layer = create_output_layer(num_neurons_per_layer[num_layers - 1]);
    if (!layer) {
        LOG_ERROR(strerror(ENOMEM));
        return false;
    }
    layers[num_layers - 1] = layer;
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
    input_layer = layers[0];
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
    double bias = 0;
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
    neural_layer_t *next_hidden_layer = NULL;
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
            LOG_ERROR(strerror(ENOMEM));
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
    const neural_layer_t *next_layer = NULL;
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
                LOG_ERROR(strerror(ENOMEM));
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
        LOG_ERROR(strerror(EINVAL));
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
        LOG_ERROR(strerror(EINVAL));
        return NULL;
    }
    if ((network->num_layers - 2) <= index) {
        LOG_ERROR(strerror(EINVAL));
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
        LOG_ERROR(strerror(EINVAL));
        return NULL;
    }
    layer = network->layers[network->num_layers - 1];
    return (layer->type == LAYER_TYPE_OUTPUT) ? layer : NULL;
}

bool train(network_t *network, nn_data_batch_t *training_data, int epochs, uint32_t num_test_per_batch, double eta, nn_data_batch_t *test_data)
{
    nn_data_batch_t *batch = NULL;
    nn_data_suite_t *suite = NULL;
    uint32_t i = 0;
    if (!network|| !training_data|| !test_data) {
        LOG_ERROR(strerror(EINVAL));
        return false;
    }
    // shuffle training_data
    for (i = 0; i < epochs; i++) {
        suite = nn_divide_batch_into_suite(training_data, num_test_per_batch);
        if (!suite) {
            LOG_ERROR("Failed to create divide the training batch");
            return false;
        }
        if (!backprop(network, suite, eta)) {
            LOG_ERROR("Failed to update the mini batch");
            return false;
        }
        if (!evaluate(test_data)) {
            LOG_ERROR("Failed to evalute test data");
            return false;
        }
    }
    return true;
}

bool evaluate(nn_data_batch_t *testing_data)
{
    return true;
}

bool backprop(network_t *network, nn_data_suite_t *training_suite, double learning_rate)
{
    uint32_t i = 0;
    for (i = 0; i < training_suite->num_batch; i++) {
        __backprop_training_batch(network, &training_suite->batches[i], learning_rate);
    }
    return true;
}

bool __backprop_training_batch(network_t *network, nn_data_batch_t *training_batch, double learning_rate)
{
    neural_layer_t *delta_layers = NULL;
    double learning_rate_per_batch = 0;
    uint32_t i = 0;
    uint32_t j = 0;
    matrix_list_t *main_bias_list = NULL;
    matrix_list_t *main_weight_list = NULL;
    matrix_list_t *delta_bias_list = NULL;
    matrix_list_t *delta_weight_list = NULL;
    matrix_list_t *weight_list = NULL;
    matrix_list_t *bias_list = NULL;
    if (!__create_matrix_list_of_bias_and_weights(network,
                &main_bias_list, &main_weight_list, false)) {
        LOG_ERROR("Failed to create zeroed list of matrices for bias and weights");
        return false;
    }
    for (i = 0; i < training_batch->num_data; i++) {
        if (!__backprop_training_data(network,
                    &training_batch->data[i], &delta_bias_list, &delta_weight_list)) {
            LOG_ERROR("Failed backpropagation");
            mtxl_destroy_list(main_bias_list);
            mtxl_destroy_list(main_weight_list);
            return false;
        }
        for (j = 0; j < delta_bias_list->num_matrix; j++) {
            matrix_t *sum = NULL;
            sum = mtx_add(main_bias_list->matrix_list[j], delta_bias_list->matrix_list[j]);
            mtx_destroy_matrix(main_bias_list->matrix_list[j]);
            main_bias_list->matrix_list[j] = sum;
        }
        for (j = 0; j < delta_weight_list->num_matrix; j++) {
            matrix_t *sum = NULL;
            sum = mtx_add(main_weight_list->matrix_list[j], delta_weight_list->matrix_list[j]);
            mtx_destroy_matrix(main_weight_list->matrix_list[j]);
            main_weight_list->matrix_list[j] = sum;
        }
    }
    mtxl_destroy_list(delta_bias_list);
    mtxl_destroy_list(delta_weight_list);
    learning_rate_per_batch = learning_rate / training_batch->num_data;
    if (!__create_matrix_list_of_bias_and_weights(network,
                &bias_list, &weight_list, true)) {
        LOG_ERROR("Failed to create the bias and weight matrix");
        return false;
    }
    for (i = 0; i < weight_list->num_matrix; i++) {
        matrix_t *sum = NULL;
        mtx_multiply_by_single_value(main_weight_list->matrix_list[i], learning_rate_per_batch);
        sum = mtx_subtract(weight_list->matrix_list[i],main_weight_list->matrix_list[i]);
        mtx_destroy_matrix(weight_list->matrix_list[i]);
        weight_list->matrix_list[i] = sum;
    }
    for (i = 0; i < bias_list->num_matrix; i++) {
        matrix_t *sum = NULL;
        mtx_multiply_by_single_value(main_bias_list->matrix_list[i], learning_rate_per_batch);
        sum = mtx_subtract(bias_list->matrix_list[i],main_bias_list->matrix_list[i]);
        mtx_destroy_matrix(bias_list->matrix_list[i]);
        bias_list->matrix_list[i] = sum;
    }
    mtxl_destroy_list(main_bias_list);
    mtxl_destroy_list(main_weight_list);
    __update_bias_and_weights(network, bias_list, weight_list);
    mtxl_destroy_list(bias_list);
    mtxl_destroy_list(weight_list);
    return true;
}

bool __backprop_training_data(network_t *network, nn_data_t *training_data,
        matrix_list_t **delta_bias_list, matrix_list_t **delta_weight_list)
{
    matrix_list_t *activation_list = NULL;
    matrix_list_t *output_list = NULL;
    
    if (!__feed_forward_for_backprop(network,
                training_data, &activation_list, &output_list)) {
        LOG_ERROR("Failed to feed forward training_data to the neural network");
        return false;
    }
    if (!__backprop_outputs_and_activations(network, training_data,
                activation_list, output_list, delta_bias_list, delta_weight_list)) {
        LOG_ERROR("Failed to backprop with output and activation values");
        mtxl_destroy_list(activation_list);
        mtxl_destroy_list(output_list);
        return false;
    }
    mtxl_destroy_list(activation_list);
    mtxl_destroy_list(output_list);
    return true;

}

bool __feed_forward_for_backprop(network_t *network, nn_data_t *training_data,
        matrix_list_t **activations, matrix_list_t **outputs)
{
    matrix_list_t *activation_matrix_list= NULL;
    matrix_list_t *output_matrix_list = NULL;
    matrix_t *activation_matrix = NULL;
    uint32_t i = 0;

    activation_matrix_list = mtxl_create_matrix_list();
    if (!activation_matrix_list) {
        LOG_ERROR("Failed to create activation matrix list");
        return false;
    }
    output_matrix_list = mtxl_create_matrix_list();
    if (!output_matrix_list) {
        LOG_ERROR("Failed to create output matrix list");
        return false;
    }
    // the inputs make up the first layer of activation vector
    activation_matrix = nn_data_to_matrix(training_data);
    if (!activation_matrix) {
        LOG_ERROR("Failed to create a activation matrix from the training data");
        return false;
    }
    for (i = 0; i < network->num_layers - 1; i++) {
        matrix_t *weight_activation_dot_matrix = NULL;
        matrix_t *output_matrix = NULL;
        matrix_t *weight_matrix = NULL;
        matrix_t *bias_matrix = NULL;
        neural_layer_t *weight_layer = NULL;
        neural_layer_t *bias_layer = NULL;

        weight_layer = __get_layer_by_index(network, i);
        bias_layer = __get_layer_by_index(network, i + 1);
        weight_matrix = __create_weight_matrix(weight_layer, true);
        if (!weight_matrix) {
            LOG_ERROR("Failed to retrieve the weight_matrix");
            goto fail;
        }
        weight_activation_dot_matrix = mtx_dot(weight_matrix, activation_matrix);
        mtx_destroy_matrix(weight_matrix);
        if (!weight_activation_dot_matrix) {
            LOG_ERROR("Failed to dot prodcut");
            goto fail;
        }
        bias_matrix = __create_bias_matrix(bias_layer, true);
        if (!bias_matrix) {
            LOG_ERROR("Failed to create a bias matrix");
            goto fail;
        }
        output_matrix = mtx_add(weight_activation_dot_matrix, bias_matrix);
        mtx_destroy_matrix(weight_activation_dot_matrix);
        mtx_destroy_matrix(bias_matrix);
        if (!output_matrix) {
            LOG_ERROR("Failed to sum the dot product and the bias matrix");
            goto fail;
        }
        mtxl_add_matrix(output_matrix_list, output_matrix);
        mtxl_add_matrix(activation_matrix_list, activation_matrix);
        activation_matrix = __apply_sigmoid(output_matrix);
    }
    mtxl_add_matrix(activation_matrix_list, activation_matrix);
    *activations = activation_matrix_list;
    *outputs = output_matrix_list;
    return true;
fail:
    mtxl_destroy_list(activation_matrix_list);
    mtxl_destroy_list(output_matrix_list);
    mtx_destroy_matrix(activation_matrix);
    return false;
}

bool __backprop_outputs_and_activations(network_t *network, nn_data_t *training_data,
        matrix_list_t *activation_list, matrix_list_t *output_list,
        matrix_list_t ** bias_list_changes, matrix_list_t **weight_list_changes)
{
    matrix_t *activation_delta_vector = NULL;
    matrix_t *cost_delta_vector = NULL;
    matrix_t *delta = NULL;
    matrix_list_t *delta_bias_list = NULL;
    matrix_list_t *delta_weight_list = NULL;
    matrix_t *transposed_activation_vector = NULL;
    uint32_t i = 0;
    cost_delta_vector = 
        __apply_cost_function(activation_list->matrix_list[activation_list->num_matrix - 1], training_data->label);
    if (!cost_delta_vector) {
        LOG_ERROR("Failed to get the cost derivative of the last activation vector");
        goto fail;
    }
    activation_delta_vector = __apply_sigmoid_prime(output_list->matrix_list[output_list->num_matrix - 1]);
    if (!activation_delta_vector) {
        LOG_ERROR("Failed to apply sigmoid derivative to the last output vector");
        mtx_destroy_matrix(cost_delta_vector);
        goto fail;
    }
    delta = mtx_multiply_column_vectors(cost_delta_vector, 0, activation_delta_vector, 0);
    if (!delta) {
        LOG_ERROR("Failed to multiply vectors");
        goto fail;
    }
    __create_matrix_list_of_bias_and_weights(network, &delta_bias_list, &delta_weight_list, false);
    delta_bias_list->matrix_list[delta_bias_list->num_matrix - 1] = delta;
    // tranposed the activation vector of the last hidden layer
    transposed_activation_vector = mtx_transpose(activation_list->matrix_list[activation_list->num_matrix - 2]);
    if (!transposed_activation_vector) {
        LOG_ERROR("Failed to transpose a matrix");
        goto fail;
    }
    delta_weight_list->matrix_list[delta_weight_list->num_matrix - 1] = mtx_dot(delta, transposed_activation_vector);
    if (!delta_weight_list->matrix_list[delta_weight_list->num_matrix - 1]) {
        LOG_ERROR("Failed to multiply vectors");
        goto fail;
    }

    // hidden layer back propagation
    for (i = 2; i < network->num_layers; i++) {
        neural_layer_t *layer = NULL;
        matrix_t *output_vector_prime = NULL;
        matrix_t *weight_vector = NULL;
        matrix_t *transposed_weight_vector = NULL;
        matrix_t *delta_weight_matrix = NULL;
        matrix_t *dot_matrix = NULL;
        matrix_t *temp_matrix = NULL;
        uint32_t num_rows = 0;
        // start from the last hidden layer
        layer = __get_layer_by_index(network, network->num_layers - i);
        // create a vector matrix from the weights
        weight_vector = __create_weight_matrix(layer, true);
        if (!weight_vector) {
            LOG_ERROR("Failed to create weight matrix");
            goto fail;
        }
        transposed_weight_vector = mtx_transpose(weight_vector);
        if (!transposed_weight_vector) {
            LOG_ERROR("Failed to transpose a weight vector");
            mtx_destroy_matrix(weight_vector);
            goto fail;
        }
        dot_matrix = mtx_dot(transposed_weight_vector, delta);
        if (!dot_matrix) {
            LOG_ERROR("Failed to multiply matrix");
            mtx_destroy_matrix(transposed_weight_vector);
            mtx_destroy_matrix(weight_vector);
            goto fail;
        }
        // propagate backwards from the last hidden layer
        // output_list_size - 1 to get last, -2 to get 2nd last which is the last of the hidden layers
        matrix_t *output_vector = output_list->matrix_list[output_list->num_matrix - i];
        output_vector_prime = __apply_sigmoid_prime(output_vector);
        if (!output_vector_prime) {
            LOG_ERROR("Failed to apply sigmoid derivative to the output vector");
            mtx_destroy_matrix(dot_matrix);
            mtx_destroy_matrix(transposed_weight_vector);
            mtx_destroy_matrix(weight_vector);
            goto fail;
        }
        num_rows = mtx_get_num_rows(output_vector_prime);
        temp_matrix = mtx_multiply_column_vectors(dot_matrix, 0, output_vector_prime, 0);
        if (!temp_matrix) {
            LOG_ERROR("Failed to multiply column vectors");
            mtx_destroy_matrix(output_vector_prime);
            mtx_destroy_matrix(dot_matrix);
            mtx_destroy_matrix(transposed_weight_vector);
            mtx_destroy_matrix(weight_vector);
            goto fail;
        }
        delta = temp_matrix;
        temp_matrix = mtx_transpose(activation_list->matrix_list[activation_list->num_matrix- i - 1]);
        if (!temp_matrix) {
            LOG_ERROR("Failed to transpose a matrix");
            mtx_destroy_matrix(output_vector_prime);
            mtx_destroy_matrix(dot_matrix);
            mtx_destroy_matrix(transposed_weight_vector);
            mtx_destroy_matrix(weight_vector);
            goto fail;
        }
        delta_weight_matrix = mtx_dot(delta, temp_matrix);
        if (!delta_weight_matrix) {
            LOG_ERROR("Failed to multiply matrix");
            mtx_destroy_matrix(temp_matrix);
            mtx_destroy_matrix(output_vector_prime);
            mtx_destroy_matrix(dot_matrix);
            mtx_destroy_matrix(transposed_weight_vector);
            mtx_destroy_matrix(weight_vector);
            goto fail;
        }
        mtx_destroy_matrix(temp_matrix);
        mtx_destroy_matrix(output_vector_prime);
        mtx_destroy_matrix(dot_matrix);
        mtx_destroy_matrix(transposed_weight_vector);
        mtx_destroy_matrix(weight_vector);
        delta_bias_list->matrix_list[network->num_layers - 1 - i] = delta;
        delta_weight_list->matrix_list[network->num_layers - 1 - i] = delta_weight_matrix;
    }
    mtx_destroy_matrix(cost_delta_vector);
    mtx_destroy_matrix(activation_delta_vector);
    mtxl_destroy_list(delta_bias_list);
    mtxl_destroy_list(delta_weight_list);
    *bias_list_changes = delta_bias_list;
    *weight_list_changes = delta_weight_list;
    return true;


fail:
    mtx_destroy_matrix(cost_delta_vector);
    mtx_destroy_matrix(activation_delta_vector);
    mtx_destroy_matrix(delta);
    mtxl_destroy_list(delta_bias_list);
    mtxl_destroy_list(delta_weight_list);
    return false;
}

bool __create_matrix_list_of_bias_and_weights(network_t *network,
        matrix_list_t **bias_matrix_list, matrix_list_t **weight_matrix_list, bool copy_values)
{
    uint32_t i = 0;
    matrix_list_t *bias_list = NULL;
    matrix_list_t *weight_list = NULL;

    bias_list = mtxl_create_matrix_list();
    weight_list = mtxl_create_matrix_list();
    if (!bias_list || !weight_list) {
        LOG_ERROR("Failed to create matrix list");
        return false;
    }
    for (i = 0; i < network->num_layers - 1; i++) {
        neural_layer_t *bias_layer = NULL;
        neural_layer_t *weight_layer = NULL;
        matrix_t *weight_matrix_to_append = NULL;
        matrix_t *bias_matrix_to_append = NULL;

        weight_layer = __get_layer_by_index(network, i);
        weight_matrix_to_append = __create_weight_matrix(weight_layer, copy_values);
        if (!weight_matrix_to_append) {
            LOG_ERROR("Failed to create a weight matrix");
            mtxl_destroy_list(weight_list);
            mtxl_destroy_list(bias_list);
            return false;
        }
        mtx_destroy_matrix(weight_matrix_to_append);
        mtxl_add_matrix(weight_list, weight_matrix_to_append);

        bias_layer = __get_layer_by_index(network, i + 1);
        bias_matrix_to_append = __create_bias_matrix(bias_layer, copy_values);
        if (!bias_matrix_to_append) {
            LOG_ERROR("Failed to create a bias matrix");
            mtxl_destroy_list(weight_list);
            mtxl_destroy_list(bias_list);
            return false;
        }
        mtxl_add_matrix(bias_list, bias_matrix_to_append);
        mtx_destroy_matrix(bias_matrix_to_append);
    }
    *bias_matrix_list = bias_list;
    *weight_matrix_list = weight_list;
    return true;
}

matrix_t *__create_weight_matrix(neural_layer_t *layer, bool copy_values)
{
    matrix_t *weight_matrix = NULL;
    uint32_t i = 0;
    if (!layer) {
        LOG_ERROR(strerror(EINVAL));
        return NULL;
    }
    switch(layer->type) {
        case LAYER_TYPE_INPUT:
            weight_matrix = mtx_create_matrix(layer->input_neurons[0]->num_weights, layer->num_neurons);
            if (!weight_matrix) {
                LOG_ERROR("Failed to create the weight matrix");
                return NULL;
            }
            if (copy_values) {
                for (i = 0; i < layer->num_neurons; i++) {
                    if (!mtx_set_column(weight_matrix, i, layer->input_neurons[i]->weights,
                                layer->input_neurons[i]->num_weights)) {
                        LOG_ERROR("Failed to set a column in the weight matrix");
                        mtx_destroy_matrix(weight_matrix);
                        return NULL;
                    }
                }
            }
           break;
        case LAYER_TYPE_HIDDEN:
            weight_matrix = mtx_create_matrix(layer->hidden_neurons[0]->num_weights, layer->num_neurons);
            if (!weight_matrix) {
                LOG_ERROR("Failed to create the weight matrix");
                return NULL;
            }
            if (copy_values) {
                for (i = 0; i < layer->num_neurons; i++) {
                    if (!mtx_set_column(weight_matrix, i, layer->hidden_neurons[i]->weights,
                                layer->hidden_neurons[i]->num_weights)) {
                        LOG_ERROR("Failed to set a column in the weight matrix");
                        mtx_destroy_matrix(weight_matrix);
                        return NULL;
                    }
                }
            }
            break;
        default:
            LOG_ERROR("Invalid layer type");
            return NULL;
    }
    return weight_matrix;
}

matrix_t *__create_bias_matrix(neural_layer_t *layer, bool copy_values)
{
    matrix_t *bias_matrix = NULL;
    uint32_t i = 0;
    if (!layer) {
        LOG_ERROR(strerror(EINVAL));
        return NULL;
    }
    switch(layer->type) {
        case LAYER_TYPE_OUTPUT:
            bias_matrix = mtx_create_matrix(layer->num_neurons, 1);
            if (!bias_matrix) {
                LOG_ERROR("Failed to create the bias matrix");
                return NULL;
            }
            if (copy_values) {
                for (i = 0; i < layer->num_neurons; i++) {
                    if (!mtx_set_cell(bias_matrix, i, 1, layer->output_neurons[i]->bias)) {
                        LOG_ERROR("Failed to set a cell in the bias matrix");
                        mtx_destroy_matrix(bias_matrix);
                        return NULL;
                    }
                }
            }
           break;
        case LAYER_TYPE_HIDDEN:
            bias_matrix = mtx_create_matrix(layer->num_neurons, 1);
            if (!bias_matrix) {
                LOG_ERROR("Failed to create the bias matrix");
                return NULL;
            }
            if (copy_values) {
                for (i = 0; i < layer->num_neurons; i++) {
                    if (!mtx_set_cell(bias_matrix, i, 1, layer->hidden_neurons[i]->bias)) {
                        LOG_ERROR("Failed to set a cell in the bias matrix");
                        mtx_destroy_matrix(bias_matrix);
                        return NULL;
                    }
                }
            }
            break;
        default:
            LOG_ERROR("Invalid layer type");
            return NULL;
    }
    return bias_matrix;
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


bool __update_bias_and_weights(network_t *network, matrix_list_t *bias_list, matrix_list_t *weight_list)
{
    return true;
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


matrix_t *__apply_sigmoid(matrix_t *matrix)
{
    matrix_t *output_matrix = NULL;
    uint32_t num_columns = 0;
    uint32_t num_rows = 0;
    double value = 0;
    uint32_t i = 0;
    uint32_t j = 0;

    num_rows = mtx_get_num_rows(matrix);
    num_columns = mtx_get_num_columns(matrix);
    output_matrix = mtx_create_matrix(num_rows, num_columns);
    if (!output_matrix) {
        LOG_ERROR("Failed to create matrix");
        return NULL;
    }
    for (i = 0; i < num_rows; i++) {
        for (j = 0; j < num_columns; j++) {
            mtx_at(matrix, i, j, &value);
            value = 1.0 / (1.0 + exp(-value));
            mtx_set_cell(output_matrix, i, j, value);
        }
    }
    return output_matrix;
}

matrix_t *__apply_sigmoid_prime(matrix_t *matrix)
{
    matrix_t *sigmoid_matrix = NULL;
    matrix_t *output_matrix = NULL;
    uint32_t num_columns = 0;
    uint32_t num_rows = 0;
    double value = 0;
    uint32_t i = 0;
    uint32_t j = 0;

    num_rows = mtx_get_num_rows(matrix);
    num_columns = mtx_get_num_columns(matrix);
    sigmoid_matrix = __apply_sigmoid(matrix);
    if (!sigmoid_matrix) {
        LOG_ERROR("Failed to apply sigmoid prime");
        return NULL;
    }
    output_matrix = mtx_create_matrix(num_rows, num_columns);
    if (!output_matrix) {
        LOG_ERROR("Failed to create matrix");
        return NULL;
    }
    for (i = 0; i < num_rows; i++) {
        for (j = 0; j < num_columns; j++) {
            mtx_at(sigmoid_matrix, i, j, &value);
            value = (1 - value) * value;
            mtx_set_cell(output_matrix, i, j, value);
        }
    }
    mtx_destroy_matrix(sigmoid_matrix);
    return output_matrix;
}

matrix_t *__create_label_matrix(uint32_t label, uint32_t num_rows)
{
    matrix_t *label_matrix = NULL;
    uint32_t i = 0;
    uint32_t j = 0;

    label_matrix = mtx_create_matrix(num_rows, 1);
    if (!label_matrix) {
        LOG_ERROR("Failed to create matrix");
        return NULL;
    }
    mtx_set_cell(label_matrix, label, 0, 1.0);
}

matrix_t *__apply_cost_function(matrix_t *matrix, uint32_t label)
{
    matrix_t *output_matrix = NULL;
    matrix_t *label_matrix = NULL;
    uint32_t num_rows = 0;

    if (mtx_get_num_columns(matrix) != 1) {
        LOG_ERROR(strerror(EINVAL));
        return NULL;
    }
    num_rows = mtx_get_num_rows(matrix);
    label_matrix = __create_label_matrix(label, num_rows);
    if (!label_matrix) {
        LOG_ERROR("Failed to create label matrix");
        return NULL;
    }
    output_matrix = mtx_subtract(matrix, label_matrix);
    if (!output_matrix) {
        LOG_ERROR("Failed to subtract matrix");
        mtx_destroy_matrix(label_matrix);
        return NULL;
    }
    mtx_destroy_matrix(label_matrix);
    return output_matrix;
}
