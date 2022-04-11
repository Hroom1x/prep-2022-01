#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "transaction_write.h"
#include "master_write.h"
#include "file_reader.h"

#define TEST_RECORD_FILENAME "record.dat"
#define TEST_TRANSFER_FILENAME "transfer.dat"

data_t test_record = {
        2,
        "test_name",
        "surname",
        "test_address",
        "8(800)555-35-35",
        10.00,
        20.00,
        30.00,
};

data_t test_transfer = {
        1,
        .cash_payments = 755.19
};

int compare_records(data_t record1, data_t record2) {
    return (
            (record1.number == record2.number) &&
            !strcmp(record1.name, record2.name) &&
            !strcmp(record1.surname, record2.surname) &&
            !strcmp(record1.address, record2.address) &&
            !strcmp(record1.tel_number, record2.tel_number) &&
            (record1.indebtedness == record2.indebtedness) &&
            (record1.credit_limit == record2.credit_limit) &&
            (record1.cash_payments == record2.cash_payments)
            );
}

int compare_transfers(data_t transfer1, data_t transfer2) {
    return (
            (transfer1.number == transfer2.number) &&
            (transfer1.cash_payments == transfer2.cash_payments)
            );
}

int main() {
    FILE *record_file = fopen(TEST_RECORD_FILENAME, "w+");
    FILE *transfer_file = fopen(TEST_TRANSFER_FILENAME, "w+");

    record_write(record_file, &test_record);
    transaction_write(transfer_file, &test_transfer);

    rewind(record_file);
    rewind(transfer_file);

    data_t *got_record = read_from_file_record(record_file);
    data_t *got_transfer = read_from_file_transfer(transfer_file);

    fclose(record_file);
    fclose(transfer_file);

    if (!compare_records(test_record, got_record[0])) {
        free(got_record);
        free(got_transfer);
        puts("**** Records compare failure ****");
        return -1;
    }

    if (!compare_transfers(test_transfer, got_transfer[0])) {
        free(got_record);
        free(got_transfer);
        puts("**** Transfer compare failure ****");
        return -1;
    }

    free(got_record);
    free(got_transfer);

    puts("**** Compare success ****");
    return 0;
}
