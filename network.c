#include <stdbool.h>
#include <stdlib.h>

#include "neuron.h"
#define NUM_LAYERS 3

#define NUM_INPUT_NEURONS 784
#define NUM_MIDDLE_NEURONS 30
#define NUM_OUTPUT_NEURONS 10
#define MIN_NEURAL_LAYER 3

//! Internal function to initialize input layer within the neural network
bool __initialize_input_layer(neural_layer_t **, uint32_t *);
//! Internal function to initialize hidden layer within the neural network
bool __initialize_hidden_layer(neural_layer_t **, uint32_t *, uint32_t);
//! Internal function to initialize output layer within the neural network
bool __initialize_output_layer(neural_layer_t **, uint32_t *, uint32_t);

void __init_bias(network_t *network);
double __gen_random_value(double min, double max);

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

void initialize_bias(network_t *network)
{
}
void __init_bias(network_t *network)
{
    double random_value = =0;
    uint32_t layer_index = 0;
    uint32_t num_neurons = 0;
    uint32_t i = 0;
    if (!network) {
        log_error(strerror(EINVAL));
        return;
    }
    // seed random value before generating random bias
    srand(time(NULL));
    // skip the first layer because input neurons do not require bias
    for (layer_index = 1; layer_index < network->num_layers; layer_index++) {
        num_neurons = network->layers[layer_index]->num_neurons;
        for (i = 0; i < num_neurons; i++) {
            random_value = __gen_random_value(0.0, 1.0);
            network->layers[layer_index]->neurons[i].bias = random_value * 4 - 2;
            random_value = __gen_random_value(0.0, 1.0);
            network->layers[layer_index]->neurons[i].weight = random_value * 4 - 2;
        }
    }
    return;
}

double __gen_random_value(double min, double max)
{
    double range = max - min;
    double div = RAND_MAX / range;
    return min + (rand() / div);
}
