#ifndef POSTFIX_NOTATION_POSTFIX_NOTATION_H
#define POSTFIX_NOTATION_POSTFIX_NOTATION_H
#include "structures.h"
List *build_notation(List *input_list);
int calculate_notation(List *postfix_notation);
#endif