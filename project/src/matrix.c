#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

Matrix* create_matrix_from_file(const char* path_file) {
    FILE *matrix_file = NULL;
    matrix_file = fopen(path_file, "r");
    int rows = 0;
    int cols = 0;
    fscanf(matrix_file, "%d%d", &rows, &cols);
    printf("!!%d  %d\n", rows, cols);
    Matrix *table = create_matrix(rows * sizeof(double), cols * sizeof(double));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("\nrow: %d ", i);
            printf("col: %d ", j);
            fscanf(matrix_file, "%lf", &table->items[i][j]);
        }
    }
    fclose(matrix_file);
    return table;
}

Matrix* create_matrix(size_t rows, size_t cols) {
    Matrix *table = malloc(sizeof(Matrix));
    if (rows == 0 || cols == 0) {
        table = NULL;
    } else {
        table->rows = rows;
        table->cols = cols;
    }
    // for (size_t i = 0; i < rows; i += sizeof(double)) {
    //     for (size_t j = 0; j < cols; j += sizeof(double)) {
    //         printf("\nrow: %zu ", i / sizeof(double));
    //         printf("col: %zu ", j / sizeof(double));
    //         table->items[i / sizeof(double)][j / sizeof(double)] =
    //     }
    // }
    return table;
}

void free_matrix(Matrix* matrix) {
    if (matrix != NULL) {
        free(matrix->items);
        free(matrix);
    }
}
