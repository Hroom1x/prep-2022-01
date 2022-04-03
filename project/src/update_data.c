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

void update_data(FILE *Blackrecord, data_t *data_record_list, data_t *data_transfer_list) {
    char format_string_blackrecord[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string_blackrecord, FORMAT_STRING_MAX_SIZE,
             "%%-%dd%%-%ds%%-%ds%%-%ds%%%ds%%%d.%df%%%d.%df%%%d.%df\n",
             NUMBER, NAMES, NAMES, ADDRESS, TEL_NUMBER,
             DOUBLE_LENGTH, DOUBLE_ACCURACY, DOUBLE_LENGTH, DOUBLE_ACCURACY, DOUBLE_LENGTH, DOUBLE_ACCURACY);

    unsigned int id = 0;
    while (data_transfer_list[id].number != 0) {
        data_record_list[data_transfer_list[id].number - 1].credit_limit += data_transfer_list[id].cash_payments;
        ++id;
    }

    id = 0;
    while (data_record_list[id].number != 0) {
        fprintf(Blackrecord, format_string_blackrecord, data_record_list[id].number,
                data_record_list[id].name, data_record_list[id].surname, data_record_list[id].address,
                data_record_list[id].tel_number, data_record_list[id].indebtedness,
                data_record_list[id].credit_limit, data_record_list[id].cash_payments);
        ++id;
    }
}
