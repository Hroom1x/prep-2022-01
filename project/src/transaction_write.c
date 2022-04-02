#include <stdio.h>

#include "utils.h"
#include "transaction_write.h"

enum buf_sizes {
    FORMAT_STRING_MAX_SIZE = 20,
    NUMBER = 3,
    CASH_PAYMENTS_LENGTH = 6,
    CASH_PAYMENTS_ACCURACY = 2
};

void transaction_write(FILE *ofPtr, data_t *transfer) {
    char format_string[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string, FORMAT_STRING_MAX_SIZE, "%%-%dd%%-%d.%df\n",
             NUMBER, CASH_PAYMENTS_LENGTH, CASH_PAYMENTS_ACCURACY);
    fprintf(ofPtr, format_string, transfer->number, transfer->cash_payments);
}
