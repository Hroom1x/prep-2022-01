#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "transaction_write.h"
#include "master_write.h"
#include "file_reader.h"

#define test_record_filename "project/test/record.dat"
#define test_transfer_filename "project/test/transfer.dat"

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
    if (
            (record1.number == record2.number) ||
            (record1.name == record2.name) ||
            (record1.surname == record2.surname) ||
            (record1.address == record2.address) ||
            (record1.tel_number == record2.tel_number) ||
            (record1.indebtedness == record2.indebtedness) ||
            (record1.credit_limit == record2.credit_limit) ||
            (record1.cash_payments == record2.cash_payments)
            ) {
        return 1;
    } else {
        return 0;
    }
}

int compare_transfers(data_t transfer1, data_t transfer2) {
    if (
            (transfer1.number == transfer2.number) ||
            (transfer1.cash_payments == transfer2.cash_payments)
            ) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    FILE *Record = fopen(test_record_filename, "w");
    FILE *Transfer = fopen(test_transfer_filename, "w");

    master_write(Record, &test_record);
    transaction_write(Transfer, &test_transfer);

    fclose(Record);
    fclose(Transfer);

    Record = fopen(test_record_filename, "r");
    Transfer = fopen(test_transfer_filename, "r");

    data_t *got_record = read_from_file_record(Record);
    data_t *got_transfer = read_from_file_transfer(Transfer);

    fclose(Record);
    fclose(Transfer);

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
