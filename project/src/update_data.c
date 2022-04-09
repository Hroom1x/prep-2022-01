#include "update_data.h"
#include "utils.h"

enum buf_sizes {
    FORMAT_STRING_MAX_SIZE = 110,
    NUMBER = 12,
    NAMES = 11,
    ADDRESS = 16,
    TEL_NUMBER = 20,
    DOUBLE_LENGTH = 12,
    DOUBLE_ACCURACY = 2
};

int update_data(FILE *blackrecord_file, data_t *record_list, data_t *transfer_list) {
    if (!blackrecord_file || !record_list || !transfer_list) {
        return ERR_WRONG_POINTER;
    }
    char format_string_blackrecord[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string_blackrecord, FORMAT_STRING_MAX_SIZE,
             "%%-%dd%%-%ds%%-%ds%%-%ds%%%ds%%%d.%df%%%d.%df%%%d.%df\n",
             NUMBER, NAMES, NAMES, ADDRESS, TEL_NUMBER,
             DOUBLE_LENGTH, DOUBLE_ACCURACY, DOUBLE_LENGTH, DOUBLE_ACCURACY, DOUBLE_LENGTH, DOUBLE_ACCURACY);

    unsigned int id = 0;
    while (transfer_list[id].number != -1) {
        record_list[transfer_list[id].number - 1].credit_limit += transfer_list[id].cash_payments;
        ++id;
    }

    id = 0;
    while (record_list[id].number != -1) {
        fprintf(blackrecord_file, format_string_blackrecord, record_list[id].number,
                record_list[id].name, record_list[id].surname, record_list[id].address,
                record_list[id].tel_number, record_list[id].indebtedness,
                record_list[id].credit_limit, record_list[id].cash_payments);
        ++id;
    }
    return 0;
}
