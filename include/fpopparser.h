#ifndef FPOPPARSER_H
#define FPOPPARSER_H
#include <stdbool.h>
#include <machinecode.h>
#include <stdint.h>
#include <reserved.h>

bool count_operands_words(char *stmt, int lineNumber, const instruction_props *props);

#endif