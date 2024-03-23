#ifndef RESERVED_H
#define RESERVED_H

#include <stdlib.h>
#include <utils.h>
#include <stdint.h>

/* enum and a static array containing all directives */
enum
{
    MCR = 0,
    ENDMCR,
    DEFINE,
    COMMENT,
    DATA,
    STRING,
    ENTRY,
    EXTERN
};
extern const char *directives[];

bool is_reserved_word(char *cmd);
bool is_register(char *op);

/**
 * @brief Define a structure to hold properties for each instruction
 *
 */
typedef struct
{
    const char *instruction;    /* String representing the instruction */
    uint8_t opcode;             /* opcode for the instruction (in machine code) */
    uint8_t num_operands;       /* Number of operands */
    uint8_t op_src_addr_rules;  /* Addressing rules for source operand. see AR_* constants */
    uint8_t op_dest_addr_rules; /* Addressing rules for dest operand */
} instruction_props;

/* bit mask for the allowed addressing rules in each command/instruction */
#define AR_IMDT 0x1  /* Immediate */
#define AR_DRCT 0x2  /* Direct*/
#define AR_FXIND 0x4 /* Fixed index*/
#define AR_DRREG 0x8 /* Direct register */
#define AR_ALL (AR_IMDT | AR_DRCT | AR_FXIND | AR_DRREG)
#define AR_NA 0

/**
 * @brief Get the properties for a given command
 *
 * @param cmd
 * @return const instruction_props* corresponding to the cmd provided
 */
const instruction_props *get_instruction_props(char *cmd);

#endif