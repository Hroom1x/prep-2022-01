#include "update_data.h"
#include "utils.h"

int update_data(FILE *actual_data_file, data_t *record_list, data_t *transfer_list) {
    if (!actual_data_file || !record_list || !transfer_list) {
        return ERR_WRONG_POINTER;
    }
    char format_string_blackrecord[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string_blackrecord, FORMAT_STRING_MAX_SIZE,
             "%%-%dd%%-%ds%%-%ds%%-%ds%%%ds%%%d.%df%%%d.%df%%%d.%df\n",
             NUMBER, NAMES, NAMES, ADDRESS, TEL_NUMBER,
             DOUBLE_LENGTH, DOUBLE_ACCURACY, DOUBLE_LENGTH, DOUBLE_ACCURACY, DOUBLE_LENGTH, DOUBLE_ACCURACY);

    for (int id = 0; transfer_list[id].number != -1; ++id) {
        record_list[transfer_list[id].number - 1].credit_limit += transfer_list[id].cash_payments;
    }

    for (int id = 0; record_list[id].number != -1; ++id) {
        fprintf(actual_data_file, format_string_blackrecord, record_list[id].number,
                record_list[id].name, record_list[id].surname, record_list[id].address,
                record_list[id].tel_number, record_list[id].indebtedness,
                record_list[id].credit_limit, record_list[id].cash_payments);
    }
    return 0;
}
