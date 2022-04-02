#include <stdio.h>

#include "utils.h"
#include "black_record.h"

enum buf_sizes {
    FORMAT_STRING_MAX_SIZE = 110,
    NUMBER = 12,
    NAMES = 11,
    ADDRESS = 16,
    TEL_NUMBER = 20,
    DOUBLE_LENGTH = 12,
    DOUBLE_ACCURACY = 2
};

void black_record(FILE *ofPTR, FILE *ofPTR_2, FILE *Blackrecord) {
    data_t client_data, transfer;

    char format_string_blackrecord[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string_blackrecord, FORMAT_STRING_MAX_SIZE,
             "%%-%dd%%-%ds%%-%ds%%-%ds%%%ds%%%d.%df%%%d.%df%%%d.%df\n",
             NUMBER, NAMES, NAMES, ADDRESS, TEL_NUMBER,
             DOUBLE_LENGTH, DOUBLE_ACCURACY, DOUBLE_LENGTH, DOUBLE_ACCURACY, DOUBLE_LENGTH, DOUBLE_ACCURACY);

    char format_string_record[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string_record, FORMAT_STRING_MAX_SIZE,
             "%%%dd%%%ds%%%ds%%%ds%%%ds%%lf%%lf%%lf\n",
             NUMBER, NAMES, NAMES, ADDRESS, TEL_NUMBER);

    while (fscanf(ofPTR, format_string_record, &client_data.number, client_data.name,
                  client_data.surname, client_data.address, client_data.tel_number, &client_data.indebtedness,
                  &client_data.credit_limit, &client_data.cash_payments) != -1) {
        while (fscanf(ofPTR_2, "%d %lf", &transfer.number, &transfer.cash_payments) != -1) {
            if ( client_data.number == transfer.number && transfer.cash_payments != 0 ) {
                client_data.credit_limit += transfer.cash_payments;
            }
        }
        fprintf(Blackrecord, format_string_blackrecord, client_data.number,
                client_data.name, client_data.surname, client_data.address, client_data.tel_number,
                client_data.indebtedness, client_data.credit_limit, client_data.cash_payments);
        rewind(ofPTR_2);
    }
}

