#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#include "matrix.h"

#define EPSILON 0.01
//! Internal helper function to check whether two double values are the same
bool __double_equals(double, double);
typedef bool (*test_func)(void *);

typedef struct test_structure {
    char *test_name;
    test_func test;
} test_t;

bool test_1(void *data)
{
    matrix_t *matrix = NULL;
    matrix = mtx_create_matrix(0, 0);
    if (matrix) {
        return false;
    }
    return true;
}

bool test_2(void *data)
{
    matrix_t *matrix = NULL;
    matrix = mtx_create_matrix(1, 1);
    double value = 0;
    if (!matrix) {
        printf("%s", strerror(EINVAL));
        return false;
    }
    if (mtx_get_num_rows(matrix) != 1 || 
            mtx_get_num_columns(matrix) != 1) {
        printf("Invalid row or column count\n");
        mtx_destroy_matrix(matrix);
        return false;
    }
    if (!mtx_at(matrix, 0, 0, &value))  {
        printf("Failed to index\n");
        mtx_destroy_matrix(matrix);
        return false;
    }
    if (value != 0) {
        printf("Value does not match\n");
        mtx_destroy_matrix(matrix);
        return false;
    }
    if (!mtx_set_cell(matrix, 0, 0, 1.232)) {
        printf("Failed to set a cell value\n");
        mtx_destroy_matrix(matrix);
        return false;
    }
    if (mtx_set_cell(matrix, 1, 0, 1.232)) {
        printf("Indexing out of bounds\n");
        mtx_destroy_matrix(matrix);
        return false;
    }
    mtx_destroy_matrix(matrix);
    return true;
}

bool test_3(void *data)
{
    double row1[] = {1, 2, 3, 4};
    double row2[] = {1.1, 2.2, 3.3, 4.4};
    double value = 0;
    matrix_t *matrix = NULL;
    matrix = mtx_create_matrix(2, 4);
    if (!matrix) {
        return false;
    }
    if (!mtx_set_row(matrix, 0, row1, 4)) {
        printf("Failed to set the first row\n");
        return false;
    }
    if (!mtx_set_row(matrix, 1, row2, 4)) {
        printf("Failed to set the second row\n");
        return false;
    }
    if (!mtx_at(matrix, 0, 0, &value) || !__double_equals(value, 1)) return false;
    if (!mtx_at(matrix, 0, 1, &value) || !__double_equals(value, 2)) return false;
    if (!mtx_at(matrix, 0, 2, &value) || !__double_equals(value, 3)) return false;
    if (!mtx_at(matrix, 0, 3, &value) || !__double_equals(value, 4)) return false;
    if (!mtx_at(matrix, 1, 0, &value) || !__double_equals(value, 1.1)) return false;
    if (!mtx_at(matrix, 1, 1, &value) || !__double_equals(value, 2.2)) return false;
    if (!mtx_at(matrix, 1, 2, &value) || !__double_equals(value, 3.3)) return false;
    if (!mtx_at(matrix, 1, 3, &value) || !__double_equals(value, 4.4)) return false;
    mtx_destroy_matrix(matrix);
    return true;
}

bool test_4(void *data)
{
    double col1[] = {1, 2, 3};
    double col2[] = {4, 5, 6};
    double value = 0;
    matrix_t *matrix = NULL;
    matrix = mtx_create_matrix(3, 2);
    if (!matrix) {
        return false;
    }
    if (!mtx_set_column(matrix, 0, col1, 3)) {
        printf("Failed to set the first column\n");
        return false;
    }
    if (!mtx_set_column(matrix, 1, col2, 3)) {
        printf("Failed to set the second column\n");
        return false;
    }
    if (!mtx_at(matrix, 0, 0, &value) || !__double_equals(value, 1)) return false;
    if (!mtx_at(matrix, 1, 0, &value) || !__double_equals(value, 2)) return false;
    if (!mtx_at(matrix, 2, 0, &value) || !__double_equals(value, 3)) return false;
    if (!mtx_at(matrix, 0, 1, &value) || !__double_equals(value, 4)) return false;
    if (!mtx_at(matrix, 1, 1, &value) || !__double_equals(value, 5)) return false;
    if (!mtx_at(matrix, 2, 1, &value) || !__double_equals(value, 6)) return false;
    mtx_destroy_matrix(matrix);
    return true;
}

// dot product 1
bool test_5(void *data)
{
    double m1_row1[] = {13,17};
    double m1_row2[] = {1, 5};
    double m2_row1[] = {5.5, 2.6, 1.9};
    double m2_row2[] = {0, 1.23, 23.3};
    matrix_t *matrix_left = NULL;
    matrix_t *matrix_right = NULL;
    matrix_t *result = NULL;
    double value = 0;

    matrix_left = mtx_create_matrix(2, 2);
    if (!matrix_left) {
        printf("Failed to create the first matrix\n");
        goto fail;
    }
    matrix_right = mtx_create_matrix(2, 3);
    if (!matrix_right) {
        printf("Failed to create the second matrix\n");
        goto fail;
    }
    if (!mtx_set_row(matrix_left, 0, m1_row1, 2) ||
            !mtx_set_row(matrix_left, 1, m1_row2, 2)) {
        printf("Failed to set the first matrix\n");
        goto fail;
    }
    if (!mtx_set_row(matrix_right, 0, m2_row1, 3) ||
            !mtx_set_row(matrix_right, 1, m2_row2, 3)) {
        printf("Failed to set the second matrix\n");
        goto fail;
    }
    result = mtx_dot(matrix_left, matrix_right);
    if (!result) {
        printf("Failed to multiply the matrices\n");
        goto fail;
    }
    if (!mtx_at(result, 0, 0, &value) || !__double_equals(value, 71.5)) goto fail;
    if (!mtx_at(result, 0, 1, &value) || !__double_equals(value, 54.71)) goto fail;
    if (!mtx_at(result, 0, 2, &value) || !__double_equals(value, 420.8)) goto fail;
    if (!mtx_at(result, 1, 0, &value) || !__double_equals(value, 5.5)) goto fail;
    if (!mtx_at(result, 1, 1, &value) || !__double_equals(value, 8.75)) goto fail;
    if (!mtx_at(result, 1, 2, &value) || !__double_equals(value, 118.4)) goto fail;
    mtx_destroy_matrix(result);
    mtx_destroy_matrix(matrix_left);
    mtx_destroy_matrix(matrix_right);
    return true;

fail:
    mtx_destroy_matrix(result);
    mtx_destroy_matrix(matrix_left);
    mtx_destroy_matrix(matrix_right);
    return false;
}

// dot product 2
bool test_6(void *data)
{
    double m1_row1[] = {4, 5, 6};
    double m2_col1[] = {1, 2, 3};
    matrix_t *matrix_left = NULL;
    matrix_t *matrix_right = NULL;
    matrix_t *result = NULL;
    double value = 0;

    matrix_left = mtx_create_matrix(1, 3);
    if (!matrix_left) {
        printf("Failed to create the first matrix\n");
        goto fail;
    }
    matrix_right = mtx_create_matrix(3, 1);
    if (!matrix_right) {
        printf("Failed to create the second matrix\n");
        goto fail;
    }
    if (!mtx_set_row(matrix_left, 0, m1_row1, 3)) {
        printf("Failed to set the first matrix\n");
        goto fail;
    }
    if (!mtx_set_column(matrix_right, 0, m2_col1, 3)) {
        printf("Failed to set the second matrix\n");
        goto fail;
    }
    result = mtx_dot(matrix_left, matrix_right);
    if (!result) {
        printf("Failed to multiply the matrices\n");
        goto fail;
    }
    if (!mtx_at(result, 0, 0, &value) || !__double_equals(value, 32)) goto fail;
    if (mtx_get_num_rows(result) != 1) goto fail;
    if (mtx_get_num_columns(result) != 1) goto fail;
    mtx_destroy_matrix(result);
    mtx_destroy_matrix(matrix_left);
    mtx_destroy_matrix(matrix_right);
    return true;

fail:
    mtx_destroy_matrix(result);
    mtx_destroy_matrix(matrix_left);
    mtx_destroy_matrix(matrix_right);
    return false;
}

// illegal dot product
bool test_7(void *data)
{
    double m1_row1[] = {4, 5, 6};
    double m2_row1[] = {1, 2, 3};
    matrix_t *matrix_left = NULL;
    matrix_t *matrix_right = NULL;
    matrix_t *result = NULL;
    double value = 0;

    matrix_left = mtx_create_matrix(1, 3);
    if (!matrix_left) {
        printf("Failed to create the first matrix\n");
        goto fail;
    }
    matrix_right = mtx_create_matrix(1, 3);
    if (!matrix_right) {
        printf("Failed to create the second matrix\n");
        goto fail;
    }
    if (!mtx_set_row(matrix_left, 0, m1_row1, 3)) {
        printf("Failed to set the first matrix\n");
        goto fail;
    }
    if (!mtx_set_row(matrix_right, 0, m2_row1, 3)) {
        printf("Failed to set the second matrix\n");
        goto fail;
    }
    result = mtx_dot(matrix_left, matrix_right);
    if (result) {
        printf("Successful operation on illegal operands\n");
        mtx_destroy_matrix(result);
        goto fail;
    }
    mtx_destroy_matrix(matrix_left);
    return true;

fail:
    mtx_destroy_matrix(result);
    mtx_destroy_matrix(matrix_left);
    mtx_destroy_matrix(matrix_right);
    return false;
}

// addition
bool test_8(void *data)
{
    double m1_row1[] = {0, 1, 2};
    double m1_row2[] = {9, 8, 7};
    double m2_row1[] = {6, 5, 4};
    double m2_row2[] = {3, 4, 5};
    matrix_t *matrix_left = NULL;
    matrix_t *matrix_right = NULL;
    matrix_t *result = NULL;
    double value = 0;

    matrix_left = mtx_create_matrix(2, 3);
    if (!matrix_left) {
        printf("Failed to create the first matrix\n");
        goto fail;
    }
    matrix_right = mtx_create_matrix(2, 3);
    if (!matrix_right) {
        printf("Failed to create the second matrix\n");
        goto fail;
    }
    if (!mtx_set_row(matrix_left, 0, m1_row1, 3) ||
            !mtx_set_row(matrix_left, 1, m1_row2, 3)) {
        printf("Failed to set the first matrix\n");
        goto fail;
    }
    if (!mtx_set_row(matrix_right, 0, m2_row1, 3) ||
            !mtx_set_row(matrix_right, 1, m2_row2, 3)) {
        printf("Failed to set the second matrix\n");
        goto fail;
    }
    result = mtx_add(matrix_left, matrix_right);
    if (!result) {
        printf("Failed to add the matrices\n");
        goto fail;
    }
    if (!mtx_at(result, 0, 0, &value) || !__double_equals(value, 6)) goto fail;
    if (!mtx_at(result, 0, 1, &value) || !__double_equals(value, 6)) goto fail;
    if (!mtx_at(result, 0, 2, &value) || !__double_equals(value, 6)) goto fail;
    if (!mtx_at(result, 1, 0, &value) || !__double_equals(value, 12)) goto fail;
    if (!mtx_at(result, 1, 1, &value) || !__double_equals(value, 12)) goto fail;
    if (!mtx_at(result, 1, 2, &value) || !__double_equals(value, 12)) goto fail;
    mtx_destroy_matrix(result);
    mtx_destroy_matrix(matrix_left);
    mtx_destroy_matrix(matrix_right);
    return true;

fail:
    mtx_destroy_matrix(result);
    mtx_destroy_matrix(matrix_left);
    mtx_destroy_matrix(matrix_right);
    return false;
}

// subtraction
bool test_9(void *data)
{
    double m1_row1[] = {0, 1, 2};
    double m1_row2[] = {9, 8, 7};
    double m2_row1[] = {6, 5, 4};
    double m2_row2[] = {3, 4, 5};
    matrix_t *matrix_left = NULL;
    matrix_t *matrix_right = NULL;
    matrix_t *result = NULL;
    double value = 0;

    matrix_left = mtx_create_matrix(2, 3);
    if (!matrix_left) {
        printf("Failed to create the first matrix\n");
        goto fail;
    }
    matrix_right = mtx_create_matrix(2, 3);
    if (!matrix_right) {
        printf("Failed to create the second matrix\n");
        goto fail;
    }
    if (!mtx_set_row(matrix_left, 0, m1_row1, 3) ||
            !mtx_set_row(matrix_left, 1, m1_row2, 3)) {
        printf("Failed to set the first matrix\n");
        goto fail;
    }
    if (!mtx_set_row(matrix_right, 0, m2_row1, 3) ||
            !mtx_set_row(matrix_right, 1, m2_row2, 3)) {
        printf("Failed to set the second matrix\n");
        goto fail;
    }
    result = mtx_subtract(matrix_left, matrix_right);
    if (!result) {
        printf("Failed to add the matrices\n");
        goto fail;
    }
    if (!mtx_at(result, 0, 0, &value) || !__double_equals(value, -6)) goto fail;
    if (!mtx_at(result, 0, 1, &value) || !__double_equals(value, -4)) goto fail;
    if (!mtx_at(result, 0, 2, &value) || !__double_equals(value, -2)) goto fail;
    if (!mtx_at(result, 1, 0, &value) || !__double_equals(value, 6)) goto fail;
    if (!mtx_at(result, 1, 1, &value) || !__double_equals(value, 4)) goto fail;
    if (!mtx_at(result, 1, 2, &value) || !__double_equals(value, 2)) goto fail;
    mtx_destroy_matrix(result);
    mtx_destroy_matrix(matrix_left);
    mtx_destroy_matrix(matrix_right);
    return true;

fail:
    mtx_destroy_matrix(result);
    mtx_destroy_matrix(matrix_left);
    mtx_destroy_matrix(matrix_right);
    return false;
}

// transpose
bool test_10(void *data)
{
    double m1_row1[] = {1, 2, 3};
    double m1_row2[] = {4, 5, 6};
    matrix_t *matrix = NULL;
    matrix_t *result = NULL;
    double value = 0;

    matrix = mtx_create_matrix(2, 3);
    if (!matrix) {
        printf("Failed to create matrix\n");
        return false;
    }
    if (!mtx_set_row(matrix, 0, m1_row1, 3) ||
            !mtx_set_row(matrix, 1, m1_row2, 3)) {
        printf("Failed to set matrix\n");
        goto fail;
    }
    result = mtx_transpose(matrix);
    if (mtx_get_num_rows(result) != 3 ||
            mtx_get_num_columns(result) != 2) {
        printf("Transposed matrix has invalid rows and columns\n");
        goto fail;
    }
    if (!mtx_at(result, 0, 0, &value) || !__double_equals(value, 1)) goto fail;
    if (!mtx_at(result, 0, 1, &value) || !__double_equals(value, 4)) goto fail;
    if (!mtx_at(result, 1, 0, &value) || !__double_equals(value, 2)) goto fail;
    if (!mtx_at(result, 1, 1, &value) || !__double_equals(value, 5)) goto fail;
    if (!mtx_at(result, 2, 0, &value) || !__double_equals(value, 3)) goto fail;
    if (!mtx_at(result, 2, 1, &value) || !__double_equals(value, 6)) goto fail;
    mtx_destroy_matrix(matrix);
    mtx_destroy_matrix(result);
    return true;

fail:
    mtx_destroy_matrix(matrix);
    mtx_destroy_matrix(result);
    return false;
}

test_t tests[] = {
    {"test_1", test_1},
    {"test_2", test_2},
    {"test_3", test_3},
    {"test_4", test_4},
    {"test_5", test_5},
    {"test_6", test_6},
    {"test_7", test_7},
    {"test_9", test_8},
    {"test_9", test_9},
    {"test_10", test_10},
};

int main(int argc, char *argv[])
{
    uint32_t failed_test_count = 0;
    uint32_t num_tests = 0;
    uint32_t i = 0;
    bool result = false;

    num_tests = sizeof(tests) / sizeof(test_t);
    for (i = 0; i < num_tests; i++) {
        result = tests[i].test(0);
        if (!result) {
            printf("Failed test: [%s]\n", tests[i].test_name);
            failed_test_count++;
        }
    }
    printf("================================================\n\n");
    printf("Total number of tests passed: %u/%u\n", num_tests - failed_test_count, num_tests);
    return 0;
}


bool __double_equals(double a, double b)
{
    return fabs(a - b) < EPSILON;
}
