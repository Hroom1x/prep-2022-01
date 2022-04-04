#ifndef PROJECT_INCLUDE_MAIN_MODULE_H_
#define PROJECT_INCLUDE_MAIN_MODULE_H_

#include "utils.h"

int write_to_file_record(FILE *Ptr, data_t *data);
int write_to_file_transfer(FILE *Ptr, data_t *data);
data_t *read_from_file_record(FILE *Ptr);
data_t *read_from_file_transfer(FILE *Ptr);
// data_t *input_data_record();
// data_t *input_data_transfer();

#endif  // PROJECT_INCLUDE_MAIN_MODULE_H_
