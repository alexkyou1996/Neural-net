//! Struct to describe input neurons
typedef struct input_neuron_struct {
    //! number of weights connected to other neurons
    uint32_t num_weights;
    //! dynamically allocated array of weights
    double *weights;
} input_neuron_t;

//! Struct to describe hidden neurons
typedef struct hidden_neuron_struct {
    //! number of weights connected to other neurons
    uint32_t num_weights;
    //! dynamically allocated array of weights
    double *weights;
    //! bias value
    double bias;
} hidden_neuron_t;

//! Structure to describe output neurons
typedef struct output_neuron_struct {
    //! bias value
    double bias;
} output_neuron_t;

//! Function to create input neuron object
/*
 * @params  uint32_t            Number of weights branching off from this neuron
 *
 * @returns input_neuron_t *    The input neuron object
 */
input_neuron_t *create_input_neuron(uint32_t);

//! Function to create hidden neuron object
/*
 * @params  uint32_t            Number of weights branching off from this neuron
 *
 * @returns hidden_neuron_t *   The hidden neuron object
 */
hidden_neuron_t *create_hidden_neuron(uint32_t);

//! Function to create output neuron object
/*
 * @returns output_neuron_t *   The output neuron object
 */
output_neuron_t *create_output_neuron();

//! Funciton to destroy input neuron object
/*
 * @params  void *              The neuron object to destroy
 */
void destroy_input_neuron(void *);

//! Funciton to destroy hidden neuron object
/*
 * @params  void *              The neuron object to destroy
 */
void destroy_hidden_neuron(void *);


//! Funciton to destroy output neuron object
/*
 * @params  void *              The neuron object to destroy
 */
void destroy_output_neuron(void *);
