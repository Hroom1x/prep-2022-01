#include "utils.h"
#include "stdio.h"

size_t timer_from(int from) {
    size_t counter;
    counter = 1;
    if (from <= 0) {
        counter = 0;
    } else {
        printf("%i", from);
        for (int i = from - 1; i >= 0; --i) {
            ++counter;
            printf(" %i", i);
        }
        printf("%s", "\n");
    }
    return counter;
}

// TODO: Implement `power of` function
int custom_pow(int base, int power) {
    return 0;
}
