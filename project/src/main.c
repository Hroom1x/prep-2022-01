#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "blackRecord.h"
#include "transactionWrite.h"
#include "masterWrite.h"

int main() {
    int choice = 0;
    void masterWrite(FILE *ofPTR, Data Client);
    void transactionWrite(FILE *ofPTR, Data transfer);
    void blackRecord(FILE *ofPTR, FILE *ofPTR_2, FILE *blackrecord, Data client_data, Data transfer);
    FILE *Ptr, *Ptr_2, *blackrecord;
    Data client_data, transfer;
    printf("%s", "please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base\n");
    while (scanf("%d", &choice) != -1) {
        switch (choice) {
            case 1:
                Ptr = fopen("record.dat", "r+");
                if ( Ptr == NULL ) {
                    puts("Have no access");
                } else {
                    masterWrite(Ptr, client_data);
                    fclose(Ptr);
                }
                break;
            case 2:
                Ptr = fopen(filename, "r+");
                if ( Ptr == NULL ) {
                    puts("Have no access");
                } else {
                    transactionWrite(Ptr, transfer);
                    fclose(Ptr);
                }
                break;
            case 3:
                Ptr = fopen("record.dat", "r");
                Ptr_2 = fopen("transaction.dat", "r");
                blackrecord = fopen("blackrecord.dat", "w");

                if (Ptr == NULL || Ptr_2 == NULL) puts("exit");
                else {
                    blackRecord(Ptr, Ptr_2, blackrecord, client_data, transfer);
                    free(Ptr);
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
