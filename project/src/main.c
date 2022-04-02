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

int input_client_data(FILE *Ptr) {
    if (!Ptr) {
        return ERR_WRONG_POINTER;
    }
    data_t Client;
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
    int res = scanf(format_string, &Client.number, Client.name, Client.surname, Client.address,
                    Client.tel_number, &Client.indebtedness, &Client.credit_limit, &Client.cash_payments);
    while (res != -1) {
        if (res != NUM_CLIENT_DATA_ARGS) {
            return ERR_WRONG_INPUT;
        }
        master_write(Ptr, Client);
        res = scanf(format_string, &Client.number, Client.name, Client.surname, Client.address,
                    Client.tel_number, &Client.indebtedness, &Client.credit_limit, &Client.cash_payments);
    }
    return 0;
}

int input_transfer(FILE *Ptr) {
    if (!Ptr) {
        return ERR_WRONG_POINTER;
    }
    data_t Transfer;
    printf("%s\n%s\n",
           "1 Number account: ",
           "2 Client cash payments: ");
    int res = scanf("%d %lf", &Transfer.number, &Transfer.cash_payments);
    while (res != -1) {
        if (res != NUM_TRANSFER_ARGS) {
            return ERR_WRONG_INPUT;
        }
        transaction_write(Ptr, Transfer);
        res = scanf("%d %lf", &Transfer.number, &Transfer.cash_payments);
    }
    return 0;
}

int main() {
    int choice = 0;
    int res = 0;
    FILE *Ptr, *Ptr_2, *Blackrecord;
    printf("%s", "please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base\n");
    while (scanf("%d", &choice) != -1) {
        switch (choice) {
            case 1:
                Ptr = fopen(record_filename, "r+");
                if ( Ptr == NULL ) {
                    puts("Have no access");
                } else {
                    res = input_client_data(Ptr);
                    fclose(Ptr);
                    if (!res) {
                        return res;
                    }
                }
                break;
            case 2:
                Ptr = fopen(transaction_filename, "r+");
                if ( Ptr == NULL ) {
                    puts("Have no access");
                } else {
                    res = input_transfer(Ptr);
                    fclose(Ptr);
                    if (!res) {
                        return res;
                    }
                }
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
