#include <stdio.h>

#include "utils.h"
#include "masterWrite.h"

// Yo! Master Write! Lets code some stuff.
void masterWrite(FILE *ofPTR, Data Client) {
    fprintf(ofPTR, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n", Client.Number, Client.Name, Client.Surname,
            Client.address, Client.TelNumber, Client.indebtedness, Client.credit_limit, Client.cash_payments);
}
