#include <stdio.h>

#include "matrix.h"

int main(void) {
    printf("BOBE\n");
    Matrix* m = create_matrix_from_file("../vkhw/matr.dat");
    printf("\n%zu~~~%zu\n", m->rows, m->cols);
    double val = 100;
    mul_scalar(m, 10);
    get_elem(m, 1* sizeof(double), 1* sizeof(double), &val);
    printf("!!!%lf", val);
    free_matrix(m);
    return 0;
}

