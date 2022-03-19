#include "case4.h"
#include "stdio.h"

void recursive_function(int to, int current_num) {
    if (current_num == to) {
        printf("%i", to);
    } else if (to > 0) {
        printf("%i ", current_num);
        recursive_function(to, current_num + 1);
    } else {
        printf("%i ", current_num);
        recursive_function(to, current_num - 1);
    }
}
