#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char template[20] = {0};
    char source[128] = {0};
    int offset_arr[256] = {0};
    int temp_length = 0;

    FILE *input = NULL;
    char *input_file_name = NULL;

    if (argc > 1) {
        if (argc != 2) {
            printf("incorrect number of arguments: \nshould be input file name or no arguments");
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

    fgets(template, 20, input);
    // cut '\n' from the template
    template[strlen(template) - 1] = '\0';
    temp_length = strlen(template);
    // initialize start characters offset: the template length
    for (int i = 0; i < 256; i++) {
        offset_arr[i] = temp_length;
    }
    // initialize offset of template alphabet
    // last character still has offset equal template length
    for (int i = 0; i < temp_length - 1; i++) {
        offset_arr[(unsigned char) template[i]] = temp_length - 1 - i;
    }
    // current character position in the text
    int cur_text_pos = temp_length;
    // current position in the source of right template bound
    int template_bound_idx;
    // current source length
    int source_length = 0;
    // comparing characters indices:
    int i = 0;
    int j = 0;

    while (!feof(input)) {
        // get new source
        fgets(source, 128, input);
        source_length = strlen(source);
        template_bound_idx = temp_length - 1;

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
    fclose(input);
    return 0;
}
