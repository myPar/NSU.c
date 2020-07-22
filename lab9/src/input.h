#ifndef DEYKSTRA_INPUT_H
#define DEYKSTRA_INPUT_H
#include <stdio.h>
FILE *set_input(int argc, char *argv[]);
int read_int(FILE *input, long *number, char *out_of_bound_message, unsigned int left_bound, unsigned int right_bound, char is_data_end);
#endif
