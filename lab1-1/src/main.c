#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// returns character hash
int get_ch_hash(unsigned char ch, int ch_idx) {
    return (int)((ch % 3) * (pow(3, ch_idx)));
}
// recurrent calculating of current substring hash
// uses the previous substring hash
int get_substring_hash(int prev_hash, unsigned char start_ch, unsigned char end_ch, int template_length) {
    return (int)((prev_hash - get_ch_hash(start_ch, 0)) / 3) +
    get_ch_hash(end_ch,  template_length - 1);
}

int main(int argc, char *argv[]) {
    FILE *input = NULL;
    char *input_file_name = NULL;
    char source[128] = {0};
    char template[20] = {0};
    int cur_source_bound_pos = 1;

    if (argc > 1) {
        if (argc != 2) {
            printf("incorrect number of arguments: \nshould be input.txt file name or no arguments");
            exit(1);
        }
        input_file_name = argv[1];

        if (!(input = fopen(input_file_name, "rt"))) {
            printf("can't open the file for reading");
            exit(1);
        }
    }
    else {
        input = stdin;
    }
    int source_length = 0;
    int temp_length = 0;
    // current substring and template hashes variables
    int substring_hash = 0;
    int template_hash = 0;

    if (!fgets(template, 19, input)) {
        printf("can't read a string\n");
        exit(1);
    }
    // cut '\n' from the template
    template[strlen(template) - 1] = '\0';
    temp_length = (int) strlen(template);

    // start index of current substring
    int start_idx;
    // end index of current substring
    int end_idx;

    // calculate template hash
    for (int i = 0; i < temp_length; i++) {
        template_hash += get_ch_hash((unsigned char) template[i], i);
    }
    printf("%d%s", template_hash, " ");

    while (!feof(input)) {
        if (!fgets(source, 128, input)) {
            break;
        }
        source_length = (int) strlen(source);
        start_idx = 0;
        end_idx = temp_length - 1;
        substring_hash = 0;
        // calculate first substring hash
        for (int i = 0; i < temp_length; i++) {
            substring_hash += get_ch_hash((unsigned char) source[i], i);
        }

        while (end_idx <= source_length - 1) {
            if (substring_hash == template_hash) {

                for (int i = 0; i < temp_length; i++) {
                    printf("%d%s", cur_source_bound_pos + start_idx + i, " ");

                    if (template[i] != source[start_idx + i]) {
                        break;
                    }
                }
            }
            start_idx++;
            end_idx++;
            // get hash for next substring
            if (end_idx < source_length) {
                substring_hash = get_substring_hash(substring_hash,
                        (unsigned char) source[start_idx - 1], (unsigned char) source[end_idx], temp_length);
            }
        }
        // increment left bound position of current source string in the text
        cur_source_bound_pos += source_length;
    }
    return 0;
}
