#ifndef RESERVED_H
#define RESERVED_H

#include <stdlib.h>
#include <utils.h>

/* enum and a static array containing all reserved words */
enum reserved_idx
{
    MCR = 0, ENDMCR, DEFINE, COMMENT,
    DATA, STRING, ENTRY, EXTERN,
    MOV, CMP, ADD, SUB, LEA,
    NOT, CLR, INC, DEC, JMP, BNE, RED, PRN, JSR,
    RTS, HLT
};
extern char *reserved_words[];
bool is_reserved_word(char *cmd);

#endif