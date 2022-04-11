#include <stdio.h>

#include "matrix.h"

int main(void) {
    printf("BOBE\n");
    Matrix* m = create_matrix_from_file("../vkhw/matr.dat");
    printf("\n%zu~~~%zu\n", m->rows, m->cols);
    free_matrix(m);
    return 0;
}

