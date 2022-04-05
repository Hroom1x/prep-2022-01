#include <stdlib.h>

#include "file_reader.h"

enum buf_sizes {
    FORMAT_STRING_MAX_SIZE = 110,
    NAME = 20,
    SURNAME = 20,
    ADDRESS = 30,
    TEL_NUMBER = 15,
};

data_t *read_from_file_record(FILE *Ptr) {
    if (!Ptr) {
        return NULL;
    }
    size_t data_size = sizeof(data_t);
    data_t *data_array = malloc(data_size);
    char format_string[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string, FORMAT_STRING_MAX_SIZE, "%%i%%%ds%%%ds%%%ds%%%ds%%lf%%lf%%lf\n",
             NAME, SURNAME, ADDRESS, TEL_NUMBER);
    unsigned int id = 0;
    while (fscanf(Ptr, format_string, &data_array[id].number, data_array[id].name,
                  data_array[id].surname, data_array[id].address,
                  data_array[id].tel_number, &data_array[id].indebtedness,
                  &data_array[id].credit_limit, &data_array[id].cash_payments) != -1) {
        data_size += sizeof(data_t);
        void *tmp = realloc(data_array, data_size);
        if (tmp == NULL) {
            free(data_array);
            return NULL;
        } else {
            data_array = tmp;
        }
        ++id;
    }
    data_array[id].number = -1;
    return data_array;
}

data_t *read_from_file_transfer(FILE *Ptr) {
    if (!Ptr) {
        return NULL;
    }
    size_t data_size = sizeof(data_t);
    data_t *data_array = malloc(data_size);
    char format_string[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string, FORMAT_STRING_MAX_SIZE, "%%i%%lf\n");
    unsigned int id = 0;
    while (fscanf(Ptr, format_string, &data_array[id].number, &data_array[id].cash_payments) != -1) {
        data_size += sizeof(data_t);
        void *tmp = realloc(data_array, data_size);
        if (tmp == NULL) {
            free(data_array);
            return NULL;
        } else {
            data_array = tmp;
        }
        ++id;
    }
    data_array[id].number = -1;
    return data_array;
}
