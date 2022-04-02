#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "black_record.h"
#include "transaction_write.h"
#include "master_write.h"

// TODO(me): написать тест

enum buf_sizes {
    FORMAT_STRING_MAX_SIZE = 110,
    NAME = 20,
    SURNAME = 20,
    ADDRESS = 30,
    TEL_NUMBER = 15,

    NUM_CLIENT_DATA_ARGS = 8,
    NUM_TRANSFER_ARGS = 2,

    ERR_WRONG_INPUT = -101,
    ERR_WRONG_POINTER = -102
};

int write_to_file_record(FILE *Ptr, data_t *data) {
    if (!Ptr) {
        return ERR_WRONG_POINTER;
    }
    master_write(Ptr, data);
    return 0;
}

int write_to_file_transfer(FILE *Ptr, data_t *data) {
    if (!Ptr) {
        return ERR_WRONG_POINTER;
    }
    transaction_write(Ptr, data);
    return 0;
}

int read_from_file(const char *filename, data_t *data_array) {
    if (!filename) {
        return ERR_WRONG_POINTER;
    }
    if (!data_array) {
        return ERR_WRONG_INPUT;
    }
    int data_size = 1;
    data_array = malloc(data_size * sizeof(data_t));
    char format_string[FORMAT_STRING_MAX_SIZE];
    snprintf(format_string, FORMAT_STRING_MAX_SIZE, "%%i%%%ds%%%ds%%%ds%%%ds%%lf%%lf%%lf\n",
             NAME, SURNAME, ADDRESS, TEL_NUMBER);
    FILE *Ptr = fopen(filename, "r");
    while (fscanf(Ptr, format_string, &data_array[data_size - 1].number, data_array[data_size - 1].name,
                  data_array[data_size - 1].surname, data_array[data_size - 1].address,
                  data_array[data_size - 1].tel_number, &data_array[data_size - 1].indebtedness,
                  &data_array[data_size - 1].credit_limit, &data_array[data_size - 1].cash_payments) != -1) {
        data_array = realloc(data_array, data_size++);
    }
    return 0;
}

data_t *input_data_record() {
    data_t *data;
    data = malloc(sizeof(data_t));
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
        return NULL;
    }
}

data_t *input_data_transfer() {
    data_t *data;
    data = malloc(sizeof(data_t));
    printf("%s\n%s\n",
           "1 Number account: ",
           "2 Client cash payments: ");
    if (scanf("%d %lf", &data->number, &data->cash_payments) != -1) {
        return data;
    } else {
        return NULL;
    }
}

/* int input_client_data(FILE *Ptr) {
    if (!Ptr) {
        return ERR_WRONG_POINTER;
    }
    data_t *Client;
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
    int res = scanf(format_string, &Client->number, Client->name, Client->surname, Client->address,
                    Client->tel_number, &Client->indebtedness, &Client->credit_limit, &Client->cash_payments);
    while (res != -1) {
        if (res != NUM_CLIENT_DATA_ARGS) {
            return ERR_WRONG_INPUT;
        }
        master_write(Ptr, Client);
        res = scanf(format_string, &Client->number, Client->name, Client->surname, Client->address,
                    Client->tel_number, &Client->indebtedness, &Client->credit_limit, &Client->cash_payments);
    }
    return 0;
} */

int main() {
    int choice = 0;
    int res = 0;
    FILE *Ptr, *Ptr_2, *Blackrecord = NULL;
    data_t /* *data_array, */ *record;
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
                        write_to_file_record(Ptr, record);
                        record = input_data_record();
                    }
                }
                fclose(Ptr);
                break;
            case 2:
                Ptr = fopen(transaction_filename, "a");
                if ( Ptr == NULL ) {
                    puts("Have no access");
                } else {
                    record = input_data_transfer();
                    while (record != NULL) {
                        write_to_file_transfer(Ptr, record);
                        record = input_data_transfer();
                    }
                }
                fclose(Ptr);
                break;
            case 3:
                Ptr = fopen(record_filename, "r");
                Ptr_2 = fopen(transaction_filename, "r");
                Blackrecord = fopen(blackrecord_filename, "w");

                if (Ptr == NULL || Ptr_2 == NULL) {
                    puts("exit");
                } else {
                    black_record(Ptr, Ptr_2, Blackrecord);
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
    return res;
}
