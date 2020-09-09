typedef struct matrix_struct {
    uint32_t num_columns;
    uint32_t num_rows;
    double **cells;
} matrix_t;

matrix_t *mtx_create_matrix(uint32_t, uint32_t);

void mtx_destroy_matrix(void *);

matrix_t *mtx_dot_product(matrix_t *, matrix_t *);

matrix_t *mtx_add(matrix_t *, matrix_t *);

matrix_t *mtx_subtract(matrix_t *, matrix_t *);

matrix_t *mtx_tranpose(matrix_t *, matrix_t *);

double mtx_at(matrix_t *, uint32_t, uint32_t);

bool mtx_set_cell(matrix_t *, uint32_t, uint32_t, double);
