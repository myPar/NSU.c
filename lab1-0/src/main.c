#include <stdio.h>
#include <string.h>
#include "input.h"

enum{temp_size = 20, src_size = 128, range = 256};

int main(int argc, char *argv[]) {
    char template[temp_size] = {0};
    char source[src_size] = {0};
    int offset_arr[range] = {0};
    int temp_length = 0;
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
    temp_length = strlen(template);
    // initialize start characters offset: the template length
    for (int i = 0; i < range; i++) {
        offset_arr[i] = temp_length;
    }
    // initialize offset of template alphabet
    // last character still has offset equal template length
    for (int i = 0; i < temp_length - 1; i++) {
        offset_arr[(unsigned char) template[i]] = temp_length - 1 - i;
    }
    // current character position in the text
    int cur_text_pos = temp_length;
    // comparing characters indices:
    int i = 0;
    int j = 0;

    while (!feof(input)) {
        // get new source
        if (!fgets(source, src_size, input)) {
            break;
        }
        // current source length
        int source_length = (int) strlen(source);
        // current position in the source of right template bound
        int template_bound_idx = temp_length - 1;

        while (template_bound_idx <= source_length - 1) {
            // set start positions of comparing characters
            i = template_bound_idx;     // in source
            j = temp_length - 1;            // in template

            while(1) {
                printf("%d%s", cur_text_pos - (temp_length - j - 1), " ");

                if (source[i] != template[j] || j <= 0) {
                    break;
                }
                i--;
                j--;
            }
            int offset = offset_arr[(unsigned char) source[template_bound_idx]];
            template_bound_idx += offset;
            cur_text_pos += offset;
        }
    }
    unset_input(input);

    return 0;
}
