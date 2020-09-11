//! Forward declartion for the matrix object
typedef struct matrix_struct matrix_t;

//! Function to create the matrix object
/*
 * @params  uint32_t            Number of rows
 * @params  uint32_t            Number of columns
 *
 * returns  matrix_t *          The matrix object
 *
 *  NOTE: I can probably implement this in single array and
 *        create more efficient algorithm. Check if this bottlenecks later
 */
matrix_t *mtx_create_matrix(uint32_t, uint32_t);

//! Function to free the matrix object
/*
 * @params void *               The matrix object
 */
void mtx_destroy_matrix(void *);

//! Function to perform matrix dot product operation
/*
 * @params  matrix_t *          The left operand
 * @params  matrix_t *          The right operand
 *
 * @returns matrix_t *          The product
 */
matrix_t *mtx_dot_product(matrix_t *, matrix_t *);

//! Function to perform matrix addition
/*
 * @params  matrix_t *          The left operand
 * @params  matrix_t *          The right operand
 *
 * @returns matrix_t *          The result
 */
matrix_t *mtx_add(matrix_t *, matrix_t *);

//! Function to perform matrix subtraction
/*
 * @params  matrix_t *          The left operand
 * @params  matrix_t *          The right operand
 *
 * @returns matrix_t *          The result
 */
matrix_t *mtx_subtract(matrix_t *, matrix_t *);

//! Function to transpose a matrix
/*
 * @params  matrix_t *          The matrix to transpose
 *
 * @returns matrix_t *          The tranposed matrix
 */
matrix_t *mtx_transpose(matrix_t *);

//! Function to retrieve a value at specific index of a matrix
/*
 * @params  matrix_t *          The matrix
 * @params  uint32_t            The row index
 * @params  uint32_t            The column index
 * @params  double *            The buffer to store the value
 *
 * @returns bool                Whether success
 */
bool mtx_at(matrix_t *, uint32_t, uint32_t, double *);

//! Function to set a value at specific index of a matrix
/*
 * @params  matrix_t *          The matrix
 * @params  uint32_t            The row index
 * @params  uint32_t            The column index
 * @params  double *            The value
 *
 * @returns bool                Whether success
 */
bool mtx_set_cell(matrix_t *, uint32_t, uint32_t, double);
