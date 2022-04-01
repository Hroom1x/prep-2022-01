#include <stdio.h>

#include "utils.h"
#include "black_record.h"

void blackRecord(FILE *ofPTR, FILE *ofPTR_2, FILE *blackrecord) {
    data_t client_data, transfer;
    while (fscanf(ofPTR, "%12d%11s%11s%16s%20s%lf%lf%lf", &client_data.number, client_data.name,
                  client_data.surname, client_data.address, client_data.tel_number, &client_data.indebtedness,
                  &client_data.credit_limit, &client_data.cash_payments) != -1) {
        while (fscanf(ofPTR_2, "%d %lf", &transfer.number, &transfer.cash_payments) != -1) {
            if ( client_data.number == transfer.number && transfer.cash_payments != 0 ) {
                client_data.credit_limit += transfer.cash_payments;
            }
        }
        fprintf(blackrecord, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n", client_data.number,
                client_data.name, client_data.surname, client_data.address, client_data.tel_number,
                client_data.indebtedness, client_data.credit_limit, client_data.cash_payments);
        rewind(ofPTR_2);
    }
}

