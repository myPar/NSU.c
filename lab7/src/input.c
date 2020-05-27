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

// reads integer value from input stream
// supports reading exception checking
// if reading successful returns 0, otherwise returns 1
int read_int(FILE *input, int *number, char *out_of_bound_message, int left_bound, int right_bound) {
    if (!fscanf(input, "%d", number)) {
        printf("can't read integer value\n");
        exit(1);
    }
    if (feof(input)) {
        printf("bad number of lines");
        return 1;
    }
    if (*number < left_bound || *number > right_bound) {
        printf("%s", out_of_bound_message);
        return 1;
    }
    return 0;
}

