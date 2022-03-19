#include "case4.h"
#include "stdio.h"

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
