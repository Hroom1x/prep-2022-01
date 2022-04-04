#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "transaction_write.h"
#include "master_write.h"
#include "update_data.h"

//TODO(ME): Дописать в список из read_from_file замыкающий нулевой элемент

enum buf_sizes {
    FORMAT_STRING_MAX_SIZE = 110,
    NAME = 20,
    SURNAME = 20,
    ADDRESS = 30,
    TEL_NUMBER = 15,
};

int write_to_file_record(FILE *Ptr, data_t *data) {
    if (!Ptr) {
        return ERR_WRONG_POINTER;
    }
    return master_write(Ptr, data);
}

int write_to_file_transfer(FILE *Ptr, data_t *data) {
    if (!Ptr) {
        return ERR_WRONG_POINTER;
    }
    return transaction_write(Ptr, data);
}

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
            data_array = NULL;
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
            data_array = NULL;
        } else {
            data_array = tmp;
        }
        ++id;
    }
    data_array[id].number = -1;
    return data_array;
}

data_t *input_data_record() {
    data_t *data = malloc(sizeof(data_t));
    char format_string[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string, FORMAT_STRING_MAX_SIZE, "%%i%%%ds%%%ds%%%ds%%%ds%%lf%%lf%%lf\n",
             NAME, SURNAME, ADDRESS, TEL_NUMBER);
    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
           "1 Number account: ",
           "2 Client name: ",
           "3 Surname: ",
           "4 address client: ",
           "5 Client Telnum: ",
           "6 Client indebtedness: ",
           "7 Client credit limit: ",
           "8 Client cash payments: ");
    if (scanf(format_string, &data->number, data->name, data->surname, data->address,
          data->tel_number, &data->indebtedness, &data->credit_limit, &data->cash_payments) != -1) {
        return data;
    } else {
        free(data);
        return NULL;
    }
}

data_t *input_data_transfer() {
    data_t *data = malloc(sizeof(data_t));
    printf("%s\n%s\n",
           "1 Number account: ",
           "2 Client cash payments: ");
    if (scanf("%d %lf", &data->number, &data->cash_payments) != -1) {
        return data;
    } else {
        free(data);
        return NULL;
    }
}

int main() {
    int choice = 0;
    FILE *Ptr, *Ptr_2, *Blackrecord = NULL;
    data_t *record;
    printf("%s", "please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base\n");
    while (scanf("%d", &choice) != -1) {
        switch (choice) {
            case 1:
                Ptr = fopen(record_filename, "a");
                if ( Ptr == NULL ) {
                    puts("Have no access");
                } else {
                    record = input_data_record();
                    while (record != NULL) {
                        if (write_to_file_record(Ptr, record) != 0) {
                            return ERR_WRONG_POINTER;
                        }
                        free(record);
                        record = input_data_record();
                    }
                    fclose(Ptr);
                }
                break;
            case 2:
                Ptr = fopen(transaction_filename, "a");
                if ( Ptr == NULL ) {
                    puts("Have no access");
                } else {
                    record = input_data_transfer();
                    while (record != NULL) {
                        if (write_to_file_transfer(Ptr, record) != 0) {
                            return ERR_WRONG_POINTER;
                        }
                        free(record);
                        record = input_data_transfer();
                    }
                    fclose(Ptr);
                }
                break;
            case 3:
                Ptr = fopen(record_filename, "r");
                Ptr_2 = fopen(transaction_filename, "r");
                Blackrecord = fopen(blackrecord_filename, "w");
                if (Ptr == NULL || Ptr_2 == NULL) {
                    puts("exit");
                } else {
                    data_t *data_record_list = read_from_file_record(Ptr);
                    data_t *data_transfer_list = read_from_file_transfer(Ptr_2);
                    update_data(Blackrecord, data_record_list, data_transfer_list);
                    free(data_record_list);
                    free(data_transfer_list);
                    fclose(Ptr);
                    fclose(Ptr_2);
                    fclose(Blackrecord);
                }
                break;
            default:
                puts("error");
                break;
        }
        printf("%s", "please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base\n");
    }
    return 0;
}
