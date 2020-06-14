# include <stdio.h>
#include <stdlib.h>

void copy_data(FILE *input, FILE *output) {
    char *buffer = (char*) malloc(sizeof(char) * 128);

    while(!feof(input)) {
        size_t n_bytes = fread(buffer, 1, sizeof(buffer), input);
        fwrite(buffer, 1, n_bytes, output);
    }
    fseek(input, 0, SEEK_SET);
    free(buffer);
}
