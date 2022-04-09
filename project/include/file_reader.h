#ifndef PROJECT_INCLUDE_FILE_READER_H_
#define PROJECT_INCLUDE_FILE_READER_H_

#include "utils.h"

data_t *read_from_file_record(FILE *record_file);
data_t *read_from_file_transfer(FILE *transfer_file);

#endif  // PROJECT_INCLUDE_FILE_READER_H_
