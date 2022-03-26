#ifndef UTILS_H
#define UTILS_H

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

/*
// This comment is very useful
printf("This message is very useful too")
 */

#endif //UTILS_H
