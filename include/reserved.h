/**
 * @file reserved.h
 * @brief Reserved Words and Instruction Properties for Assembly Language.
 *
 * efines enums and structures for handling reserved words, including
 * directives, assembly commands, and register names. It provides
 * functionalities to check if a given string is a reserved word, a register
 * name, and to retrieve properties for assembly language instructions,
 * such as opcode, operand count, and addressing rules. This setup is
 * crucial for the lexical and syntactic analysis stages of an assembler.
 */
D

#ifndef RESERVED_H
#define RESERVED_H

#include <stdlib.h>
#include <utils.h>
#include <stdint.h>

    /**
     * @brief enum and a static array containing all supported directives
     *
     */
    enum {
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

/**
 * @brief Find out whether string given in parameter is a reserved word. A reserved word could be a one of the
 * supported assembly commands, a directive (e.g. .define, .extern etc..) or a name of a register (r0..r7).
 *
 * @param cmd string to check
 * @return true cmd is a reserved word
 * @return false cmd is not a reserved word
 */
bool is_reserved_word(char *cmd);

/**
 * @brief Returns whether op is a valid register name. Valid names are "r0" to "r7". Case sensitive.
 *
 * @param op operand string
 * @return true valid register name
 * @return false not a register name
 */
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

/** bit mask for the allowed addressing types in each command/instruction */
#define AR_IMDT 0x1                                      /*!< Immediate */
#define AR_DRCT 0x2                                      /*!< Direct*/
#define AR_FXIND 0x4                                     /*!< Fixed index*/
#define AR_DRREG 0x8                                     /*!< Direct register */
#define AR_ALL (AR_IMDT | AR_DRCT | AR_FXIND | AR_DRREG) /*!< All addressing types supported */
#define AR_NA 0                                          /*!< None of the addressing types allwoed */

/**
 * @brief Get the properties for a given command
 *
 * @param cmd
 * @return const instruction_props* corresponding to the cmd provided
 */
const instruction_props *get_instruction_props(char *cmd);

#endif
