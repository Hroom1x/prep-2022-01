#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

Matrix* create_matrix_from_file(const char* path_file) {
    FILE *matrix_file = fopen(path_file, "r");
    if (!matrix_file) {
        return NULL;
    }
    int rows = 0;
    int cols = 0;
    fscanf(matrix_file, "%d%d", &rows, &cols);
    Matrix *table = create_matrix(rows * sizeof(double), cols * sizeof(double));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(matrix_file, "%lf", &table->items[i + j]);
        }
    }
    fclose(matrix_file);
    return table;
}

Matrix* create_matrix(size_t rows, size_t cols) {
    Matrix *table = malloc(sizeof(Matrix));
    if (rows == 0 || cols == 0) {
        free(table);
        return NULL;
    } else {
        table->rows = rows;
        table->cols = cols;
        table->items = malloc(rows * cols / sizeof(double));
        return table;
    }
}

void free_matrix(Matrix* matrix) {
    if (matrix != NULL) {
        free(matrix->items);
        free(matrix);
    }
}

int get_rows(const Matrix* matrix, size_t* rows) {
    if (!matrix) {
        return 0;
    } else {
        *rows = (size_t) matrix->rows;
        return 1;
    }
}

int get_cols(const Matrix* matrix, size_t* cols) {
    if (!matrix) {
        return 0;
    } else {
        *cols = (size_t) matrix->cols;
        return 1;
    }
}

int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
    if (!matrix) {
        return 0;
    }
    *val = matrix->items[row + col];
    return 1;
}

int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
    if (!matrix) {
        return 0;
    }
    matrix->items[row + col] = val;
    return 1;
}

Matrix* mul_scalar(const Matrix* matrix, double val) {
    if (!matrix) {
        return NULL;
    }

    for (size_t i = 0; i < matrix->rows; i += sizeof(double)) {
        for (size_t j = 0; j < matrix->cols; j += sizeof(double)) {
            printf("++%zu++%zu\n", i, j);
            matrix->items[i + j] *= val;
        }
    }

    return (Matrix *) matrix;
}
