#include "stdio.h"
#include <stdlib.h>

#include "utils.h"
#include "case3.h"
#include "case4.h"

#define ERR_ARGS_COUNT (-1)
#define ERR_WRONG_FLG (-2)

#define TST_FOO_FIX     1
#define TST_FOO_IMPL    2
#define TST_MOD_IMPL    3
#define TST_CASE4_IMPL  4

int main(int argc, const char** argv) {
    if (argc < 3) {
        return ERR_ARGS_COUNT;
    }

    int Test_case = atoi(argv[1]);
    const char* data;
    data = argv[2];

    switch (Test_case) {
        case TST_FOO_FIX: {
            int to = atoi(data);
            if (to > 0) {
                size_t ticks_count = timer_from(to);
                printf("%zu\n", ticks_count);
            } else {
                printf("%i", 0);
            }
            break;
        }
        case TST_FOO_IMPL: {
            if (argc == 4) {
                int base = atoi(data);
                int pow =  atoi(argv[3]);
                int res = custom_pow(base, pow);

                printf("%i\n", res);
            } else {
                return ERR_ARGS_COUNT;
            }
            break;
        }
        case TST_MOD_IMPL: {
            int num = atoi(data);
            int res = is_prime(num);
            printf("%i", res);
            break;
        }
        case TST_CASE4_IMPL: {
            int to = atoi(data);
            recursive_function(to);
            break;
        }
        default: {
            return ERR_WRONG_FLG;
        }
    }
}
