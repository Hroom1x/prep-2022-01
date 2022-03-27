#include <stdio.h>

#include "utils.h"
#include "transactionWrite.h"

void transactionWrite(FILE *ofPtr, Data transfer) {
    fprintf(ofPtr, "%-3d%-6.2f\n", transfer.Number, transfer.cash_payments);
}
