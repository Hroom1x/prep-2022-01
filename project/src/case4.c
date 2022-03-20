#include "stdio.h"

#include "case4.h"

void recursive_function(int to) {
    if (to == 1) {
        printf("%i", 1);
    } else if (to > 1) {
        recursive_function(to - 1);
        printf(" %i", to);
    } else if (to < 1) {
        recursive_function(to + 1);
        printf(" %i", to);
    }
}
