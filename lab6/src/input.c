#include <stdio.h>
#include <stdlib.h>

FILE *set_input(int argc, char *argv[]) {
    FILE *input = NULL;

    if (argc > 1) {
        if (argc != 2) {
            printf("incorrect number of arguments: \nshould be input file name or no arguments");
            exit(1);
        }
        char *input_file_name;
        input_file_name = argv[1];

        if (!(input = fopen(input_file_name, "rt"))) {
            printf("can't open the file for reading");
            exit(1);
        }
    }
    else {
        input = stdin;
    }

    return input;
}
