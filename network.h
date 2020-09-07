//! Function to create and initialize a neural network object
/*
 * @params  uint32_t *          Number of neurons to allocate at each layer
 * @params  uint32_t            Number of layers to allocate
 *
 * @returns network_t           The neural network object
 */
network_t create_network(uint32_t *, uint32_t);


//! The sigmoid activation function for neuron firing
/*
 * @param double **     2D Matrix of neurons
 * @param size_t        Matrix row size
 * @param size_t        Matrix column size
 *
 * returns              2D Matrix of neurons with activation function applied
 */
double **sigmoid(double **, size_t, size_t);

//! The tanh activation function for neuron firing
/*
 * @param double **     2D Matrix of neurons
 * @param size_t        Matrix row size
 * @param size_t        Matrix column size
 *
 * returns              2D Matrix of neurons with activation function applied
 */
double **tanh(double **, size_t, size_t);

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
 * @param double ****   The training data
 * @param int           The epoch
 * @param int           The batch size
 * @param double        ETA
 * @param double ****   The test data
 */
void SGD(double ****, int, uint32_t, double, double ****);


//! Function to update training data
/*
 * @param double ****   Training data
 * @param double        ETA
 */
void update_mini_batches(double ****, double);

//! Function to find the difference between actual and desired
//! and change weights accordingly
/*
 * @param double **     
 * @param double **
 *
 * @returns double ****
 */
double ****back_prop(double **, double **);