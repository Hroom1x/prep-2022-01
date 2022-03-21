#include "stdio.h"

#include "case2.h"

int custom_pow(int base, int power) {
    int res = 1;
    for (int i = power; i > 0; --i) {
        res *= base;
    }
    return res;
}
