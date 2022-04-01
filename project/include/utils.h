#ifndef PROJECT_INCLUDE_UTILS_H_
#define PROJECT_INCLUDE_UTILS_H_

#include <stdio.h>

#define filename "transaction.dat"
// enum file_names {
//     //
// };

struct master_record {
    int number;
    char name[20];
    char surname[20];
    char address[30];
    char tel_number[15];
    double indebtedness;
    double credit_limit;
    double cash_payments;
};

typedef struct master_record data_t;

#endif  // PROJECT_INCLUDE_UTILS_H_
