#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_module.h"
#include "utils.h"
#include "transaction_write.h"
#include "master_write.h"
#include "update_data.h"
#include "test_module.h"

#define test_record_filename "test_record.dat"
#define test_transfer_filename "test_transfer.dat"

enum buf_sizes {
    FORMAT_STRING_MAX_SIZE = 110,
    NAME = 20,
    SURNAME = 20,
    ADDRESS = 30,
    TEL_NUMBER = 15,
};

void test_write_to_file() {
    FILE *Record = fopen(test_record_filename, "w");
    FILE *Transfer = fopen(test_transfer_filename, "w");
    data_t *test_record = malloc(sizeof(data_t));
    data_t *test_transfer = malloc(sizeof(data_t));

    test_record->number = 1;
    snprintf(test_record->name, NAME, "name_1");
    snprintf(test_record->surname, NAME, "surname_1");
    snprintf(test_record->address, ADDRESS, "address_1");
    snprintf(test_record->tel_number, TEL_NUMBER, "tel_number_1");
    test_record->indebtedness = 10.0;
    test_record->credit_limit = 20.0;
    test_record->cash_payments = 30.0;

    test_transfer->number = 1;
    test_transfer->cash_payments = 40.0;

    write_to_file_record(Record, test_record);
    write_to_file_transfer(Transfer, test_transfer);

    data_t *got_record = &read_from_file_record(Record)[0];
    data_t *got_transfer = &read_from_file_transfer(Transfer)[0];

    if (
            got_record->number == test_record->number ||
            got_record->name == test_record->name ||
            got_record->surname == test_record->surname ||
            got_record->address == test_record->address ||
            got_record->tel_number == test_record->tel_number ||
            got_record->indebtedness == test_record->indebtedness ||
            got_record->credit_limit == test_record->credit_limit ||
            got_record->cash_payments == test_record->cash_payments ||
            got_transfer->number == test_transfer->number ||
            got_transfer->cash_payments == test_transfer->cash_payments
            ) {
        printf("---- TEST MODULE SUCCESS ----\n");
    } else {
        printf("---- TEST MODULE FAILED ----\n");
        printf("%i == %i", got_record->number, test_record->number);
    }
    free(test_record);
    free(test_transfer);
}
