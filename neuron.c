#include <stdlib.h>
#include "neuron.h"

//! Function to create input neuron object
/*
 * @params  uint32_t            Number of weights branching off from this neuron
 *
 * @returns input_neuron_t *    The input neuron object
 */
input_neuron_t *create_input_neuron(uint32_t num_weights)
{
    input_neuron_t *neuron = NULL;
    neuron = calloc(sizeof(input_neuron_t), 1);
    if (!neuron) {
        LOG_ERROR(strerror(ENOMEM));
        return NULL;
    }
    if (num_weights) {
        neuron->weights = calloc(sizeof(double), num_weights);
        if (!neuron->weights) {
            LOG_ERROR(strerror(ENOMEM));
            destroy_input_neuron(neuron);
            return NULL;
        }
        neuron->num_weights = num_weights;
    }
    return neuron;
}

//! Function to create hidden neuron object
/*
 * @params  uint32_t            Number of weights branching off from this neuron
 *
 * @returns hidden_neuron_t *   The hidden neuron object
 */
hidden_neuron_t *create_hidden_neuron(uint32_t num_weights)
{
    input_neuron_t *neuron = NULL;
    neuron = calloc(sizeof(hidden_neuron_t), 1);
    if (!neuron) {
        LOG_ERROR(strerror(ENOMEM));
        return NULL;
    }
    if (num_weights) {
        neuron->weights = calloc(sizeof(double), num_weights);
        if (!neuron->weights) {
            LOG_ERROR(strerror(ENOMEM));
            destroy_hidden_neuron(neuron);
            return NULL;
        }
        neuron->num_weights = num_weights;
    }
    return neuron;
}

//! Function to create output neuron object
/*
 * @returns output_neuron_t *   The output neuron object
 */
output_neuron_t *create_output_neuron()
{
    return calloc(sizeof(output_neuron_t), 1);
}

//! Funciton to destroy input neuron object
/*
 * @params  void *              The neuron object to destroy
 */
void destroy_input_neuron(void *neuron)
{
    input_neuron_t *input_neuron = (input_neuron_t *)neuron;
    if (input_neuron->weights) {
        free(input_neuron->weights);
    }
    free(input_neuron);
}

//! Funciton to destroy hidden neuron object
/*
 * @params  void *              The neuron object to destroy
 */
void destroy_hidden_neuron(void *neuron)
{
    hidden_neuron_t *hidden_neuron = (hidden_neuron_t *)neuron;
    if (hidden_neuron->weights) {
        free(hidden_neuron->weights);
    }
    free(hidden_neuron);
}

//! Funciton to destroy output neuron object
/*
 * @params  void *              The neuron object to destroy
 */
void destroy_output_neuron(void *neuron)
{
    free(neuron);
}
