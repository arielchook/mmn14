#ifndef OP_PARSER_H
#define OP_PARSER_H

#include <reserved.h>

/* Operands separator character */
#define OP_SEPARATOR ','

bool parse_operands(char *stmt, int lineNumber, const instruction_props *props);

#endif