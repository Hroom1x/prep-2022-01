#include "stdio.h"

#include "case3.h"

int is_prime(int num) {
    if (num == 1 || num == 0) {
        return 0;
    } else if (num < 0) {
        return 0;
    } else {
        for (int i = num - 1; i > 1; --i) {
            if (num % i == 0) {
                return 0;
            }
        }
    }
    return 1;
}
