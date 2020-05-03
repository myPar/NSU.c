//
// Created by Andrew on 3/2/2020.
//
#include "change_base.h"
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int get_value(char ch) {   // gets 10-base value of the number
    if (!isdigit(ch)) {
        return ch - 'a' + 10;
    }
    else {
        return ch - '0';
    }
}

char get_number(int number) {
    return (number <= 9) ? ((char)number + '0') : ((char)number - 10 + 'a');
}

double get_base10_number(char* number, int base) { // returns double value of input number //cut get10
    int int_part_size = 1;
    int number_len = strlen(number);
    double result = 0;

    if (number[0] != '0') {     // check non zero int part consistence
        int_part_size = 0;

        for (int i = 0; i < number_len; i++) {
            if (number[i] == '.') {
                break;
            }
            int_part_size++;
        }
    }
    // int_part_size equals zero so we have no int part in number divided with '.'
    if (int_part_size == 0) {
        printf("%s", "bad input");
        exit(0);
    }
    // we have one character after int part, so it can be only '.' and we have no fractal part
    if (int_part_size + 1 == number_len) {
        printf("%s", "bad input");
        exit(0);
    }
    for (int i = 0; i < int_part_size; i++) {   // increment result on int part
        result += get_value(number[i])*pow(base,(int_part_size - i - 1));
    }

    for (int i = int_part_size + 1; i < number_len; i++) {  // increment result on float part
        result += get_value(number[i])*pow(base, -(i - int_part_size));
    }

    return result;
}

void convert_int_part(char* int_part_output, long long int_part, int base) {
    int i = 0;

    if (int_part == 0) {
        int_part_output[i] = '0';
        i++;
    }
    else {
        while (int_part > 0) {
            int_part_output[i++] = get_number(int_part % base);
            int_part /= base;
        }
    }
    int_part_output[i] = 0;
}

void convert_fractal_part(char* fractal_part_output, double fractal_part, int base) {

    for (int i = 0; i < 13; i++) {
        double int_part;
        fractal_part = modf(fractal_part * base, &int_part);
        char ch[2] = {get_number(int_part), 0};
        strcat(fractal_part_output, ch);
    }
}

void reverse_string(char* input_string) {
    int length = strlen(input_string);
    char ch;

    for (int i = 0; i < length / 2; i++) {
        ch = input_string[i];
        input_string[i] = input_string[length - i - 1];
        input_string[length - i - 1] = ch;
    }
}

void new_base_convert(char* output_number, double number, int base) {  // returns 10-base number, converted to input number base
    double int_part,
            fractal_part = modf(number, &int_part);

    convert_int_part(output_number, int_part, base);
    reverse_string(output_number);

    if (fractal_part > 0) {
        strcat(output_number, ".");
        convert_fractal_part(output_number, fractal_part, base);
    }
}
