#include <stdio.h>

#include "utils.h"
#include "transaction_write.h"

enum buf_sizes {
    FORMAT_STRING_MAX_SIZE = 20,
    NUMBER = 3,
    CASH_PAYMENTS_LENGTH = 6,
    CASH_PAYMENTS_ACCURACY = 2
};

int transaction_write(FILE *transaction_file, data_t *transfer) {
    if (!transaction_file || !transfer) {
        return ERR_WRONG_POINTER;
    }
    char format_string[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string, FORMAT_STRING_MAX_SIZE, "%%-%dd%%-%d.%df\n",
             NUMBER, CASH_PAYMENTS_LENGTH, CASH_PAYMENTS_ACCURACY);
    fprintf(transaction_file, format_string, transfer->number, transfer->cash_payments);
    return 0;
}
