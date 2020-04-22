#include <corecrt_wstdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

// method copy data from input file to output
void write_file_data(FILE *input, FILE *output) {
    char buffer[128];

    while(!feof(input)) {
        size_t n_bytes = fread(buffer, 1, sizeof(buffer), input);
        fwrite(buffer, 1, n_bytes, output);
    }
    fseek(input, 0, SEEK_SET);
}
// function makes name for new test data file
char *make_file_name(int test_number) {
    char *name = "../../data_of_test_";
    char *number_str = "";
    itoa(test_number, number_str, 10);
    strcat(name, number_str);
    return name;
}
// method writes zero in file with current test number if file is empty
void check_empty_data(FILE *file, char *file_name) {
    int read_number;
    size_t read_bytes = fread(&read_number, sizeof(int), 1, file);

    if (!read_bytes) {  // red zero bytes means that the file is empty
        // write zero in empty file //
        fclose(file);
        check_errno("error closing %s 1", file_name);

        file = fopen(file_name, "wb");

        if (!file) {
            fatal_sys_error("can't open file %s for writing", file_name);
        }
        int number = 0;
        fwrite(&number, sizeof(int), 1, file);
        // reopen the file //
        fclose(file);
        check_errno("error closing %s 2", file_name);
        file = fopen(file_name, "rb");

        if (!file) {
            fatal_sys_error("can't open file %s for reading", file_name);
        }
    }
    fseek(file, 0, SEEK_SET);   // move pointer to the beginning of the file
}

// method makes new file with data of the current test
void make_test_data_file(FILE *input) {
    char *fname = "../../test_number_data_file";
    FILE *test_number_data_file = fopen(fname, "rb");   // this file include the number of the current test

    if (!test_number_data_file) { // the file with test number data was deleted
        test_number_data_file = fopen(fname, "wb"); // make new file

        if (!test_number_data_file) {
            fatal_sys_error("can't open file %s for writing", fname);
        }
        fclose(test_number_data_file);
        check_errno("error closing %s 3", fname);
        test_number_data_file = fopen(fname, "rb");

        if (!test_number_data_file) {
            fatal_sys_error("can't open file %s for reading", fname);
        }
    }
    check_empty_data(test_number_data_file, fname);    // write zero if file is empty

    int test_number;
    fread(&test_number, sizeof(int), 1, test_number_data_file); // read the number of the current test - 1
    test_number++;  // increase test number

    char *test_data_file_name = make_file_name(test_number);
    FILE *test_data_file = fopen(test_data_file_name, "wb");

    if (!test_data_file) {
        fatal_sys_error("can't open file %s for writing", test_data_file_name);
    }
    write_file_data(input, test_data_file);
    // write updated test number in the file //
    fclose(test_number_data_file);
    check_errno("error closing %s 5", fname);
    test_number_data_file = fopen(fname, "wb");

    if (!test_number_data_file) {
        fatal_sys_error("can't open file %s for writing", fname);
    }
    fwrite(&test_number, sizeof(int), 1, test_number_data_file);
    //
    fclose(test_number_data_file);
    fclose(test_data_file);
}