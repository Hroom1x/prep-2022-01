#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "transaction_write.h"
#include "master_write.h"
#include "update_data.h"
#include "file_reader.h"

enum choices {
    MAKE_RECORD = 1,
    MAKE_TRANSFER,
    UPDATE_DATA
};

static data_t *input_data_record() {
    data_t *data = malloc(sizeof(data_t));
    char format_string[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string, FORMAT_STRING_MAX_SIZE, "%%i%%%ds%%%ds%%%ds%%%ds%%lf%%lf%%lf\n",
             NAMES, NAMES, ADDRESS, TEL_NUMBER);
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

static data_t *input_data_transfer() {
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
    FILE *record_file, *transfer_file, *actual_file = NULL;
    data_t *record;
    printf("%s", "please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base\n");
    while (scanf("%d", &choice) != -1) {
        switch (choice) {
            case MAKE_RECORD:
                record_file = fopen(RECORD_FILENAME, "a");
                if (record_file == NULL) {
                    puts("Have no access");
                } else {
                    record = input_data_record();
                    while (record != NULL) {
                        if (master_write(record_file, record) != 0) {
                            return ERR_WRONG_POINTER;
                        }
                        free(record);
                        record = input_data_record();
                    }
                    fclose(record_file);
                }
                break;
            case MAKE_TRANSFER:
                transfer_file = fopen(TRANSACTION_FILENAME, "a");
                if (transfer_file == NULL) {
                    puts("Have no access");
                } else {
                    record = input_data_transfer();
                    while (record != NULL) {
                        if (transaction_write(transfer_file, record) != 0) {
                            return ERR_WRONG_POINTER;
                        }
                        free(record);
                        record = input_data_transfer();
                    }
                    fclose(transfer_file);
                }
                break;
            case UPDATE_DATA:
                record_file = fopen(RECORD_FILENAME, "r");
                transfer_file = fopen(TRANSACTION_FILENAME, "r");
                actual_file = fopen(ACTUAL_RECORD_FILENAME, "w");
                if (record_file == NULL || transfer_file == NULL) {
                    puts("exit");
                } else {
                    data_t *data_record_list = read_from_file_record(record_file);
                    data_t *data_transfer_list = read_from_file_transfer(transfer_file);
                    update_data(actual_file, data_record_list, data_transfer_list);
                    free(data_record_list);
                    free(data_transfer_list);
                    fclose(record_file);
                    fclose(transfer_file);
                    fclose(actual_file);
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
