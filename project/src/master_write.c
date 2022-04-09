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

int master_write(FILE *record_file, data_t *client) {
    if (!record_file || !client) {
        return ERR_WRONG_POINTER;
    }
    char format_string[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string, FORMAT_STRING_MAX_SIZE,
             "%%-%dd%%-%ds%%-%ds%%-%ds%%%ds%%%d.%df%%%d.%df%%%d.%df\n",
             NUMBER, NAMES, NAMES, ADDRESS, TEL_NUMBER,
             DOUBLE_LENGTH, DOUBLE_ACCURACY, DOUBLE_LENGTH, DOUBLE_ACCURACY, DOUBLE_LENGTH, DOUBLE_ACCURACY);
    fprintf(record_file, format_string, client->number, client->name,
            client->surname, client->address, client->tel_number, client->indebtedness, client->credit_limit,
            client->cash_payments);
    return 0;
}
