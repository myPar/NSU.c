#define _CRT_SECURE_NO_WARNINGS
#ifndef LAB0_INPUT_H
#define LAB0_INPUT_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
bool is_correct_number(int number_base, char number[]);

int get_input(int *arg1_pointer, int *arg2_pointer, char** arg3_pointer);

#endif //LAB0_INPUT_H