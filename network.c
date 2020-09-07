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
    neural_layer_t *layer = NULL;
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
    network->num_layers = num_layers;
    network->layers = layers;
    __initialize_bias_and_weights(network);
    return network;
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
