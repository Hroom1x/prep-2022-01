#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "transaction_write.h"
#include "master_write.h"
#include "update_data.h"
#include "file_reader.h"

enum buf_sizes {
    FORMAT_STRING_MAX_SIZE = 110,
    NAME = 20,
    SURNAME = 20,
    ADDRESS = 30,
    TEL_NUMBER = 15,
};

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
    FILE *ptr, *ptr_2, *blackrecord = NULL;
    data_t *record;
    printf("%s", "please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base\n");
    while (scanf("%d", &choice) != -1) {
        switch (choice) {
            case 1:
                ptr = fopen(record_filename, "a");
                if (ptr == NULL) {
                    puts("Have no access");
                } else {
                    record = input_data_record();
                    while (record != NULL) {
                        if (master_write(ptr, record) != 0) {
                            return ERR_WRONG_POINTER;
                        }
                        free(record);
                        record = input_data_record();
                    }
                    fclose(ptr);
                }
                break;
            case 2:
                ptr = fopen(transaction_filename, "a");
                if (ptr == NULL) {
                    puts("Have no access");
                } else {
                    record = input_data_transfer();
                    while (record != NULL) {
                        if (transaction_write(ptr, record) != 0) {
                            return ERR_WRONG_POINTER;
                        }
                        free(record);
                        record = input_data_transfer();
                    }
                    fclose(ptr);
                }
                break;
            case 3:
                ptr = fopen(record_filename, "r");
                ptr_2 = fopen(transaction_filename, "r");
                blackrecord = fopen(actual_record_filename, "w");
                if (ptr == NULL || ptr_2 == NULL) {
                    puts("exit");
                } else {
                    data_t *data_record_list = read_from_file_record(ptr);
                    data_t *data_transfer_list = read_from_file_transfer(ptr_2);
                    update_data(blackrecord, data_record_list, data_transfer_list);
                    free(data_record_list);
                    free(data_transfer_list);
                    fclose(ptr);
                    fclose(ptr_2);
                    fclose(blackrecord);
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
