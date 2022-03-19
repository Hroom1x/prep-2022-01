#include "case3.h"
#include "stdio.h"
#include <math.h>

int is_prime(signed int num) {
    int max_possible_div = round(sqrt(sqrt(num*num)));
    if (num == 1 || num == 0) {
        return 0;
    } else {
        for (int i = max_possible_div; i > 0; --i) {
            if (num % i == 0) {
                return 0;
            }
        }
    }
    return 1;
}
