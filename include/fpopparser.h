#ifndef FPOPPARSER_H
#define FPOPPARSER_H
#include <stdbool.h>
#include <machinecode.h>
#include <stdint.h>
#include <reserved.h>

/* Operands separator character */
#define OP_SEPARATOR ','

/* Imeediate values must start with this prefix */
#define IMMEDIATE_VALUE_PREFIX "#"

/* Array subscription character */
#define ARRAY_OPEN_CHAR '['
/* Array suffix */
#define ARRAY_CLOSE_CHAR "]"

bool count_operands_words(char *stmt, int lineNumber, const instruction_props *props);

#endif