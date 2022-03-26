#include <stdio.h>

#include "utils.h"
#include "masterWrite.h"

// Yo! Master Write! Lets code some stuff.
void masterWrite(FILE *ofPTR, Data Client) {
    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
           "1 Number account: ",
           "2 Client name: ",
           "3 Surname: ",
           "4 address client: ",
           "5 Client Telnum: ",
           "6 Client indebtedness: ",
           "7 Client credit limit: ",
           "8 Client cash payments: ");
    while (scanf("%d%s%s%s%s%lf%lf%lf", &Client.Number, Client.Name, Client.Surname, Client.address, Client.TelNumber,
                 &Client.indebtedness, &Client.credit_limit, &Client.cash_payments) != -1) {
        fprintf(ofPTR, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n", Client.Number, Client.Name, Client.Surname,
                Client.address, Client.TelNumber, Client.indebtedness, Client.credit_limit, Client.cash_payments);
        printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
               "1 Number account: ",
               "2 Client name: ",
               "3 Surname: ",
               "4 address client: ",
               "5 Client Telnum: ",
               "6 Client indebtedness: ",
               "7 Client credit limit: ",
               "9 Client cash payments:"
        );
    }
}
