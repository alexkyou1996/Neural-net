

// NOTE: I can probably implement this in single array and
//       create more efficient algorithm. Check if this bottlenecks later
matrix_t *mtx_create_matrix(uint32_t rows, uint32_t columns)
{
    matrix_t *matrix = NULL;
    uint32_t i = 0;
    if (!rows || !columns) {
        log_error(strerror(EINVAL));
        return NULL;
    }
    matrix = calloc(sizeof(matrix_t), 1);
    if (!matrix) {
        log_error(strerror(ENOMEM));
        return NULL;
    }
    matrix->cells = calloc(sizeof(double *), rows);
    if (!matrix->cells) {
        log_error(strerror(ENOMEM));
        mtx_destroy_matrix(matrix);
        return NULL;
    }
    for (i = 0; i < rows, i++) {
        matrix->cells[i] = calloc(sizeof(double), columns);
        if (!matrix->cells[i]) {
            log_error(strerror(ENOMEM));
            mtx_destroy_matrix(matrix);
            return NULL;
        }
    }
    return matrix;
}

void mtx_destroy_matrix(void *matrix)
{
    uint32_t i = 0;
    matrix_t *m = (matrix_t *)matrix;
    for (i = 0; i < m->num_rows; i++) {
        free(m->cells[i]
    }
    free(m->cells);
    free(m);
}

matrix_t *mtx_dot(matrix_t *matrix_left1, matrix_t *matrix_right)
{
    matrix_t *product = NULL;
}

matrix_t *mtx_add(matrix_t *, matrix_t *);

matrix_t *mtx_subtract(matrix_t *, matrix_t *);

matrix_t *mtx_tranpose(matrix_t *, matrix_t *);

double mtx_at(matrix_t *, uint32_t, uint32_t);

bool mtx_set_cell(matrix_t *, uint32_t, uint32_t, double);
