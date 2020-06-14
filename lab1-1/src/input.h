#pragma once
#ifndef RABINKARP_INPUT_H
#define RABINKARP_INPUT_H
#include <stdio.h>
FILE *set_input(int argc, char *argv[]);
void unset_input(FILE *input);
#endif
