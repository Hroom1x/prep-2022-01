#include "main_module.h"

void test_write_to_file() {
    const char *filename = "....";

    struct some expected_data;
    // Fill `expected_data` with some test values
    // and write them into the file `f`.
    // ...
    write_to_file(filename, &expected_data);

    struct some got_data;
    read_from_file(filename, &got_data);

    // Compare any field from `expected_data` with any field from `got_data`
    // ....
}
