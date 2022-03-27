#ifndef PROJECT_INCLUDE_UTILS_H_
#define PROJECT_INCLUDE_UTILS_H_

#include <stdio.h>

#define filename "transaction.dat"

struct masterRecord {
    int Number;
    char Name[20];
    char Surname[20];
    char address[30];
    char TelNumber[15];
    double indebtedness;
    double credit_limit;
    double cash_payments;
};
typedef struct masterRecord Data;

#endif  // PROJECT_INCLUDE_UTILS_H_
