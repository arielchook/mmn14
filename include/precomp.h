#ifndef PRECOMP_H
#define PRECOMP_H

#include <utils.h>

/* Preprocess a .am file to generate a .as file where all macros are expanded */
bool precompile(FILE *input, FILE *output);

#endif