#include <stdio.h>
#include <string.h>
#include "input.h"

// returns character hash
int get_ch_hash(unsigned char ch, int ch_idx) {
    int pow = 1;
    for (int i = 0; i < ch_idx; i++) {
        pow *= 3;
    }
    return (ch % 3) * pow;
}
// recurrent calculating of current substring hash
// uses the previous substring hash
int get_substring_hash(int prev_hash, unsigned char start_ch, unsigned char end_ch, int template_length) {
    return (prev_hash - get_ch_hash(start_ch, 0)) / 3 +
    get_ch_hash(end_ch,  template_length - 1);
}

enum{buff_size = 128, temp_size = 20};

int main(int argc, char *argv[]) {
    char source[buff_size] = {0};
    char template[temp_size] = {0};
    int cur_source_bound_pos = 1;
    int template_hash = 0;
    FILE *input;

    if ((input = set_input(argc, argv)) == NULL) {
        return 1;
    }
    if (!fgets(template, temp_size, input)) {
        printf("can't read a string\n");
        return 1;
    }
    // cut '\n' from the template
    template[strlen(template) - 1] = '\0';
    int temp_length = (int) strlen(template);

    // calculate template hash
    for (int i = 0; i < temp_length; i++) {
        template_hash += get_ch_hash((unsigned char) template[i], i);
    }
    printf("%d%s", template_hash, " ");

    while (!feof(input)) {
        if (!fgets(source, buff_size, input)) {
            break;
        }
        int source_length = (int) strlen(source);
        int start_idx = 0;
        int end_idx = temp_length - 1;
        int substring_hash = 0;
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
    unset_input(input);

    return 0;
}
