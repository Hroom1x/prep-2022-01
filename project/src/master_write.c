#include <stdio.h>

#include "utils.h"
#include "master_write.h"

// Yo! Master Write! Lets code some stuff.
void master_write(FILE *ofPTR, data_t Client) {
    fprintf(ofPTR, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n", Client.number, Client.name,
            Client.surname, Client.address, Client.tel_number, Client.indebtedness, Client.credit_limit,
            Client.cash_payments);
}
