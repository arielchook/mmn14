#ifndef SPOPPARSER_H
#define SPOPPARSER_H

#include <reserved.h>

bool parse_operands(char *stmt, int lineNumber, const instruction_props *props);

#endif