#ifndef PROJECT_INCLUDE_UTILS_H_
#define PROJECT_INCLUDE_UTILS_H_

#include <stdio.h>

#define TRANSACTION_FILENAME "transaction.dat"
#define RECORD_FILENAME "record.dat"
#define ACTUAL_RECORD_FILENAME "actual_record.dat"

#define FORMAT_STRING_MAX_SIZE 110
#define NUMBER 12
#define NAMES 11
#define ADDRESS 16
#define TEL_NUMBER 20
#define DOUBLE_LENGTH 12
#define DOUBLE_ACCURACY 2

enum errors {
    ERR_WRONG_POINTER = -1
};

typedef struct data {
    int number;
    char name[20];
    char surname[20];
    char address[30];
    char tel_number[15];
    double indebtedness;
    double credit_limit;
    double cash_payments;
} data_t;

#endif  // PROJECT_INCLUDE_UTILS_H_
