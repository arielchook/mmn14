#ifndef OP_PARSER_H
#define OP_PARSER_H

#include <reserved.h>

/* Operands separator character */
#define OP_SEPARATOR ','

/* Imeediate values must start with this prefix */
#define IMMEDIATE_VALUE_PREFIX "#"

/* Array subscription character */
#define ARRAY_OPEN_CHAR '['
/* Array suffix */
#define ARRAY_CLOSE_CHAR "]"

bool parse_operands(char *stmt, int lineNumber, const instruction_props *props);

#endif