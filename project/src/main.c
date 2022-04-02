#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "black_record.h"
#include "transaction_write.h"
#include "master_write.h"

// TODO(me): проверка на null if (!c) {error}
// TODO(me): проверка на считывание if (scanf() != *количество параметров*) {error}
// TODO(me): вынести константы в enum
// TODO(me): написать тест

// enum buf_sizes {
//     NAME = 20 - 1,
//     SURNAME = 20 - 1,
//     ADDRESS = 30 - 1,
//     TEL_NUMBER = 15 - 1
// };

int input_client_data(data_t *Client) {
    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
           "1 Number account: ",
           "2 Client name: ",
           "3 Surname: ",
           "4 address client: ",
           "5 Client Telnum: ",
           "6 Client indebtedness: ",
           "7 Client credit limit: ",
           "8 Client cash payments: ");
    return scanf("%i%20s%20s%30s%15s%lf%lf%lf", &Client->number, Client->name, Client->surname,
                 Client->address, Client->tel_number, &Client->indebtedness, &Client->credit_limit,
                 &Client->cash_payments);
}

int input_transfer(data_t *transfer) {
    printf("%s\n%s\n",
           "1 Number account: ",
           "2 Client cash payments: ");
    return scanf("%d %lf", &transfer->number, &transfer->cash_payments);
}

int main() {
    int choice = 0;
    FILE *Ptr, *Ptr_2, *blackrecord;
    data_t client_data, transfer;
    printf("%s", "please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base\n");
    while (scanf("%d", &choice) != -1) {
        switch (choice) {
            case 1:
                Ptr = fopen(record_filename, "r+");
                if ( Ptr == NULL ) {
                    puts("Have no access");
                } else {
                    while (input_client_data(&client_data) != -1) {
                        master_write(Ptr, client_data);
                    }
                    fclose(Ptr);
                }
                break;
            case 2:
                Ptr = fopen(transaction_filename, "r+");
                if ( Ptr == NULL ) {
                    puts("Have no access");
                } else {
                    while (input_transfer(&transfer) != -1) {
                        transaction_write(Ptr, transfer);
                    }
                    fclose(Ptr);
                }
                break;
            case 3:
                Ptr = fopen(record_filename, "r");
                Ptr_2 = fopen(transaction_filename, "r");
                blackrecord = fopen(blackrecord_filename, "w");

                if (Ptr == NULL || Ptr_2 == NULL) {
                    puts("exit");
                } else {
                    blackRecord(Ptr, Ptr_2, blackrecord);
                    fclose(Ptr);
                    fclose(Ptr_2);
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
