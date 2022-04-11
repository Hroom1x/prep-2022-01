#include <stdio.h>

#include "utils.h"
#include "transaction_write.h"

#define TRANSACTION_STRING_MAX_SIZE 20
#define TRANSACTION_NUMBER 3
#define CASH_PAYMENTS_LENGTH 5

int transaction_write(FILE *transaction_file, data_t *transfer) {
    if (!transaction_file || !transfer) {
        return ERR_WRONG_POINTER;
    }
    char format_string[TRANSACTION_STRING_MAX_SIZE];
    snprintf(format_string, TRANSACTION_STRING_MAX_SIZE, "%%-%dd%%-%d.%df\n",
             TRANSACTION_NUMBER, CASH_PAYMENTS_LENGTH, DOUBLE_ACCURACY);
    fprintf(transaction_file, format_string, transfer->number, transfer->cash_payments);
    return 0;
}
