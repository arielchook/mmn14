#ifndef MCWORDTYPES_H
#define MCWORDTYPES_H

#include <stdio.h>
#include <stdint.h>

/**
 * @brief this enum has 2 purposes: first is for the supported addressing types
 * which also indicates the type of memory word in the mc_word union
 *
 */
enum addressing_type
{
    WT_IMMEDIATE = 0,
    WT_DIRECT = 1,
    WT_FIXED_INDEX = 2,
    WT_DIRECT_REG = 3,
    WT_INSTRUCTION = 4,
    WT_INVALID
};

enum abs_rloc_extern
{
    ARE_ABS = 0,
    ARE_EXTERN = 1,
    ARE_RELOC = 2
};
/**
 * @brief mc_word describes a full instruction in memory after parsing. since an instruction may consist of one
 * or more words in memory we use a union.
 *
 * note that we are not using :bits since it's not supported in ansi c for types smaller than int (e.g. char, uint8_t)
 */
typedef union
{

    /* an instruction is the first word that gets written to memory. the dest_addressing and src_addressing are
    determined by the operands */
    struct
    {
        uint8_t A_R_E;           /* 2-bit ARE */
        uint8_t dest_addressing; /* 2-bit dest operand addressing type */
        uint8_t src_addressing;  /* 2-bit source operand addressing type */
        uint8_t opcode;          /* 4-bit opcode */
    } instruction;

    /* immediate addressing memory word */
    struct
    {
        uint8_t A_R_E;  /* 2-bit ARE */
        uint16_t value; /* 12-bit value */
    } immediate;

    /* direct addressing memory word */

    struct
    {
        uint8_t A_R_E;    /* 2-bit ARE */
        uint16_t address; /* 12-bit address of label */
        char *external_symbol;
    } direct;

    /* fixed index is spanned over 2 memory words */
    struct
    {
        uint8_t A_R_E_1; /* 2-bit ARE for the 1st word */
        uint16_t array;  /* 12-bit address of array */
        uint8_t A_R_E_2; /* 2-bit ARE for the 2nd word */
        uint16_t index;  /* 12-bit array index */
        char *external_symbol;
    } fixed_index;

    /* direct register memory word */
    struct
    {
        uint8_t A_R_E; /* 2-bit ARE for the 1st word */
        uint8_t dest;  /* 3-bit dest register number */
        uint8_t src;   /* 3-bit src register number */
        uint8_t regnum;
    } direct_reg;

} mc_word_union;

typedef struct
{
    /* holds the type of memory word which will help detrmine the fields to encode */
    enum addressing_type type;

    mc_word_union contents;
} mc_word;
#endif