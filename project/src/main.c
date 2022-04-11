#include <stdio.h>

#include "matrix.h"

int main(void) {
    printf("BOBE\n");
    Matrix* m = create_matrix_from_file("matr.dat");
    free_matrix(m);
    printf("\n%zu~~~%zu\n", m->rows, m->cols);
    return 0;
}

