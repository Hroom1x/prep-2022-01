#include <stdlib.h>

#include "file_reader.h"

data_t *read_from_file_record(FILE *record_file) {
    if (!record_file) {
        return NULL;
    }
    char format_string[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string, FORMAT_STRING_MAX_SIZE, "%%i%%%ds%%%ds%%%ds%%%ds%%lf%%lf%%lf\n",
             NAMES, NAMES, ADDRESS, TEL_NUMBER);
    unsigned int id = 0;
    data_t *data_array = malloc(sizeof(data_t));
    while (fscanf(record_file, format_string, &data_array[id].number, data_array[id].name,
                  data_array[id].surname, data_array[id].address,
                  data_array[id].tel_number, &data_array[id].indebtedness,
                  &data_array[id].credit_limit, &data_array[id].cash_payments) != -1) {
        void *tmp = realloc(data_array, sizeof(data_t) * (++id + 1));
        if (tmp == NULL) {
            free(data_array);
            return NULL;
        } else {
            data_array = tmp;
        }
    }
    data_array[id].number = -1;
    return data_array;
}

data_t *read_from_file_transfer(FILE *transfer_file) {
    if (!transfer_file) {
        return NULL;
    }
    char format_string[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string, FORMAT_STRING_MAX_SIZE, "%%i%%lf\n");
    unsigned int id = 0;
    data_t *data_array = malloc(sizeof(data_t));
    while (fscanf(transfer_file, format_string,
                  &data_array[id].number, &data_array[id].cash_payments) != -1) {
        void *tmp = realloc(data_array, sizeof(data_t) * (++id + 1));
        if (tmp == NULL) {
            free(data_array);
            return NULL;
        } else {
            data_array = tmp;
        }
    }
    data_array[id].number = -1;
    return data_array;
}
