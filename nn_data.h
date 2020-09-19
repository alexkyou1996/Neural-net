#define IMAGE_WIDTH  28
#define IMAGE_HEIGHT 28

typedef struct nn_data_struct {
    double pixels[IMAGE_WIDTH*IMAGE_HEIGHT];
    uint32_t label;
} nn_data_t;

typedef struct nn_data_batch_struct {
    uint32_t num_data;
    nn_data_t *data;
    int data_type;
} nn_data_batch_t;

typedef struct nn_data_suite_struct {
    uint32_t num_batch;
    nn_data_batch_t *batches;
} nn_data_suite_t;

typedef enum nn_data_type_enum {
    NN_DATA_TRAIN = 0,
    NN_DATA_TEST,
} nn_data_type_t;
