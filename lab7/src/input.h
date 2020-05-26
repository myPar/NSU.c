#ifndef TOP_SORT_INPUT_H
#define TOP_SORT_INPUT_H
#include <stdio.h>
FILE *set_input(int argc, char *argv[]);
int read_int(FILE *input, int *number, char *out_of_bound_message, int left_bound, int right_bound);
#endif
