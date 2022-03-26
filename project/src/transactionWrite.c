#include <stdio.h>

#include "utils.h"
#include "transactionWrite.h"

void transactionWrite(FILE *ofPtr, Data transfer) {
#include"utils.h"

    printf("%s\n%s\n",
           "1 Number account: ",
           "2 Client cash payments: ");
    while (scanf("%d %lf", &transfer.Number, &transfer.cash_payments) != -1) {
        fprintf(filename, "%-3d%-6.2f\n", transfer.Number, transfer.cash_payments);
        printf("%s\n%s\n",
               "1 Number account:",
               "2 Client cash payments: "
        );
    }
}