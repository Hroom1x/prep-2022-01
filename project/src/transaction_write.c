#include <stdio.h>

#include "utils.h"
#include "transaction_write.h"

void transaction_write(FILE *ofPtr, data_t transfer) {
    fprintf(ofPtr, "%-3d%-6.2f\n", transfer.number, transfer.cash_payments);
}
