#include "stdio.h"

#include "utils.h"

size_t timer_from(size_t from) {
    size_t counter;
    counter = 1;
    printf("%zu", from);
    for (int i = from - 1; i >= 0; --i) {
        ++counter;
        printf(" %i", i);
    }
    printf("%s", "\n");
    return counter;
}
