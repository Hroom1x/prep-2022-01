#include <stdio.h>

#include "utils.h"
#include "master_write.h"

enum buf_sizes {
    FORMAT_STRING_MAX_SIZE = 110,
    NUMBER = 12,
    NAMES = 11,
    ADDRESS = 16,
    TEL_NUMBER = 20,
    DOUBLE_LENGTH = 12,
    DOUBLE_ACCURACY = 2
};

void master_write(FILE *ofPTR, data_t Client) {
    char format_string[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string, FORMAT_STRING_MAX_SIZE,
             "%%-%dd%%-%ds%%-%ds%%-%ds%%%ds%%%d.%df%%%d.%df%%%d.%df\n",
             NUMBER, NAMES, NAMES, ADDRESS, TEL_NUMBER,
             DOUBLE_LENGTH, DOUBLE_ACCURACY, DOUBLE_LENGTH, DOUBLE_ACCURACY, DOUBLE_LENGTH, DOUBLE_ACCURACY);
    fprintf(ofPTR, format_string, Client.number, Client.name,
            Client.surname, Client.address, Client.tel_number, Client.indebtedness, Client.credit_limit,
            Client.cash_payments);
}
