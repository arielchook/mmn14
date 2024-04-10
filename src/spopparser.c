/**
 * @file spopparser.c
 * @brief Function prototypes and definitions for parsing operands in the second pass of the assembler.
 *
 * @authors Ariel Cohen, Jonathan Transky
 */

#include <fpopparser.h>
#include <reserved.h>
#include <messages.h>
#include <utils.h>
#include <symbols.h>
#include <mcwordtypes.h>
#include <machinecode.h>

/**
 * @brief Handles immediate operand parsing and conversion.
 *
 * This function processes an immediate operand, converting it to an integer or resolving it as a constant.
 * It validates the value and converts it to two's complement if necessary.
 *
 * @param op Operand string starting with '#'.
 * @param lineNumber Current line number for error reporting.
 * @param word Pointer to machine code word to populate.
 * @return True on successful processing, False on error.
 */
bool handle_immediate(char *op, int lineNumber, mc_word *word)
{
    int intValue;
    uint16_t twoc_intValue;
    char *ptr;
    SymbolBlock *sb;

    /* Skip the '#' */
    op++;

    /* Try to convert to int */
    intValue = strtol(op, &ptr, 10);

    /* This means it's a string. Check if it's a name of a constant */
    if (*ptr != '\0')
    {
        sb = find_symbol(op);
        if (sb == NULL || sb->type != ST_DEFINE)
        {
            printf(ERR_CANT_FIND_DEFINE, lineNumber, op);
            return false;
        }

        /* At this point we found a constant with that name. Use its value */
        twoc_intValue = sb->value;
    }
    else
    {
        /* Make sure the int value provided can be represented in memory */
        if ((intValue < MIN_VALUE) || (intValue > MAX_VALUE))
        {
            printf(ERR_INT_OUT_OF_BOUNDS, lineNumber, intValue, MIN_VALUE, MAX_VALUE);
            return false;
        }
        /* Convert to two-complement */
        twoc_intValue = to_twos_complement(intValue);
    }

    word->type = WT_IMMEDIATE;
    word->contents.immediate.A_R_E = ARE_ABS;
    word->contents.immediate.value = twoc_intValue;
    return true;
}

/**
 * @brief Handles fixed index addressing mode.
 *
 * This function processes an operand in fixed index addressing mode, extracting the array name and index.
 * It checks the symbol table for the array and index and populates the machine code word accordingly.
 *
 * @param op Operand string with array name and index.
 * @param lineNumber Current line number for error reporting.
 * @param word Pointer to machine code word to populate.
 * @return True on successful processing, False on error.
 */
bool handle_fixed_addressing(char *op, int lineNumber, mc_word *word)
{
    char *array_name, *array_index;
    int intValue;
    char *ptr;
    SymbolBlock *sb;

    /* Chop the ']' */
    op[strlen(op) - 1] = '\0';

    /* Extract the array name and the array index. We know they exist or it would have failed in first pass */
    array_name = extractWordSeparator(op, 1, NULL, ARRAY_OPEN_CHAR);
    rtrim(array_name);
    array_index = extractWordSeparator(op, 2, NULL, ARRAY_OPEN_CHAR);

    /* Look up the array name in the symbol list */
    sb = find_symbol(array_name);
    if (sb == NULL)
    {
        printf(ERR_ARRAY_NOT_FOUND, lineNumber, array_name);
        free_if_not_null(array_name);
        free_if_not_null(array_index);
        return false;
    }

    /* External symbols vs internal symbols get a different handling */
    switch (sb->type)
    {
    case ST_EXTERN:
        word->contents.fixed_index.A_R_E_1 = ARE_EXTERN; /* External */
        word->contents.fixed_index.array = 0;
        word->contents.fixed_index.external_symbol = array_name; /* We keep the name of the symbol so we can */
                                                                 /* add it to the list of external symbols, when */
                                                                 /* updating the code section */
        break;
    case ST_DATA:
    case ST_STRING:
    case ST_CODE:
        /* First word is the array address */
        word->contents.fixed_index.A_R_E_1 = ARE_RELOC; /* Relocatable */
        /* Store the address of the array label in the word to be written to memory */
        word->contents.fixed_index.array = sb->value;
        word->contents.fixed_index.external_symbol = NULL;
        break;
    case ST_DEFINE:
        printf(ERR_DEFINE_DISALLOWED, lineNumber);
        free_if_not_null(array_name);
        free_if_not_null(array_index);
        return false;
        break;
    }

    /* Now, look at the array_index - try to convert to int */
    intValue = strtol(array_index, &ptr, 10);

    /* This means it's a string. Check if it's a name of a constant */
    if (*ptr != '\0')
    {
        sb = find_symbol(array_index);
        if (sb == NULL || sb->type != ST_DEFINE)
        {
            printf(ERR_CANT_FIND_DEFINE, lineNumber, array_index);
            free_if_not_null(array_name);
            free_if_not_null(array_index);
            return false;
        }

        /* At this point we found a constant with that name. Use its value */
        intValue = sb->value;

        /* The value from the define is given in 2 complement. Make sure it's positive since it's an array index */
        if ((sb->value & MIN_VALUE) != 0)
            intValue = -1;
    }

    /* Array index must be non negative */
    if (intValue < 0)
    {
        printf(ERR_ARR_IND_NEGATIVE, lineNumber);
        free_if_not_null(array_name);
        free_if_not_null(array_index);
        return false;
    }

    /* Second word is the array index */
    word->type = WT_FIXED_INDEX;
    word->contents.fixed_index.A_R_E_2 = ARE_ABS; /* Absolute */
    word->contents.fixed_index.index = intValue;

    free_if_not_null(array_name);
    free_if_not_null(array_index);
    return true;
}

/**
 * @brief Handles direct register addressing mode.
 *
 * This function identifies the register number and populates the machine code word accordingly.
 *
 * @param op Operand string representing a register.
 * @param lineNumber Current line number for error reporting.
 * @param word Pointer to machine code word to populate.
 * @return True on successful processing, False on error.
 */
bool handle_direct_register(char *op, int lineNumber, mc_word *word)
{
    /* Figure out which register it is */
    int reg_num = op[1] - '0';

    word->type = WT_DIRECT_REG;
    word->contents.direct_reg.A_R_E = ARE_ABS;
    /* We use reg_num since we can't tell at this point if it's a source or a dest operand.
       This will be set correctly by the calling function */
    word->contents.direct_reg.src = 0;
    word->contents.direct_reg.dest = 0;
    word->contents.direct_reg.regnum = reg_num;
    return true;
}

/**
 * @brief Handles direct addressing mode.
 *
 * This function looks up the symbol in the symbol table and populates the machine code word based on the symbol type.
 *
 * @param op Operand string representing a symbol.
 * @param lineNumber Current line number for error reporting.
 * @param word Pointer to machine code word to populate.
 * @return True on successful processing, False on error.
 */
bool handle_direct_addressing(char *op, int lineNumber, mc_word *word)
{
    SymbolBlock *sb;

    /* Look up the symbol name. By now all symbols should be defined */
    sb = find_symbol(op);
    if (sb == NULL)
    {
        printf(ERR_CANT_RESOLVE_SYMBOL, lineNumber, op);
        return false;
    }

    switch (sb->type)
    {
    case ST_DATA:
    case ST_STRING:
    case ST_CODE:
        word->type = WT_DIRECT;
        word->contents.direct.A_R_E = ARE_RELOC;   /* Relocatable symbol */
        word->contents.direct.address = sb->value; /* Address of label or data */
        word->contents.direct.external_symbol = NULL;
        break;
    case ST_EXTERN:
        word->type = WT_DIRECT;
        word->contents.direct.A_R_E = ARE_EXTERN; /* External symbol */
        word->contents.direct.address = 0;
        word->contents.direct.external_symbol = op; /* We keep the name of the symbol so we can */
                                                    /* add it to the list of external symbols, when */
                                                    /* updating the code section */
        break;
    case ST_DEFINE:
        printf(ERR_DEFINE_NO_HASHTAG, lineNumber, op);
        return false;
        break;
    }

    return true;
}

/**
 * @brief Processes an operand based on addressing rules.
 *
 * This function determines the operand's addressing mode and processes it accordingly.
 *
 * @param op The operand string to process.
 * @param address_rules Addressing rules applicable to this operand.
 * @param lineNumber Current line number for error reporting.
 * @param word Pointer to machine code word to populate.
 * @return True on successful processing, False on error.
 */
bool process_operand(char *op, uint8_t address_rules, int lineNumber, mc_word *word)
{
    /* Make sure it's not empty */
    if (strlen(op) == 0)
    {
        printf(ERR_EMPTY_OPERAND, lineNumber);
    }

    /* Debug log each operand */
    LOG("|%s", op);

    /* 1. Immediate addressing */
    if (startsWith(op, IMMEDIATE_VALUE_PREFIX))
    {
        return handle_immediate(op, lineNumber, word);
    }

    /* Fixed index addressing */
    if (endsWith(op, ARRAY_CLOSE_CHAR))
    {
        return handle_fixed_addressing(op, lineNumber, word);
    }

    /* Direct register addressing */
    if (is_register(op))
    {
        return handle_direct_register(op, lineNumber, word);
    }

    /* If none of the above, it means it's direct addressing */
    return handle_direct_addressing(op, lineNumber, word);
}

/**
 * @brief Parses operands of an assembly instruction and generates machine code words.
 *
 * This function processes source and destination operands of an assembly instruction
 * based on their addressing modes and the instruction properties. It generates and serializes
 * the machine code words for the instruction and its operands.
 *
 * @param stmt The assembly statement containing the operands.
 * @param lineNumber Current line number for error reporting.
 * @param props The properties of the instruction being parsed.
 * @return True if operands are processed successfully, False on error.
 */
bool parse_operands(char *stmt, int lineNumber, const instruction_props *props)
{
    bool success = true;
    mc_word *word_instruction = NULL, *word_src = NULL, *word_dest = NULL;

    /* Get the first operand and second operands (if any). Cannot have more since it was checked in first pass */
    char *op_src = extractWordSeparator(stmt, 1, NULL, OP_SEPARATOR);
    char *op_dest = extractWordSeparator(stmt, 2, NULL, OP_SEPARATOR);

    /* If we have only 1 operand, it's the dest operand */
    if (props->num_operands == 1)
    {
        op_dest = op_src;
        op_src = NULL;
    }

    /* Prepare the first word of the instruction and then depending on the command, operands, and addressing,
       we need to prepare the source operand word and dest operand word for memory encoding */

    /* First word is the instruction word. It will always exist */
    word_instruction = safe_malloc(sizeof(mc_word));
    word_instruction->type = WT_INSTRUCTION;
    word_instruction->contents.instruction.A_R_E = ARE_ABS;
    word_instruction->contents.instruction.opcode = props->opcode;

    /* Is there a source operand? */
    if (success && op_src != NULL)
    {
        word_src = safe_malloc(sizeof(mc_word));
        success = process_operand(op_src, props->op_src_addr_rules, lineNumber, word_src);
        if (success)
        {
            /* Update the source operand addressing type */
            word_instruction->contents.instruction.src_addressing = word_src->type;

            /* If operand is direct reg, need to store the register number in source reg */
            if (word_src->type == WT_DIRECT_REG)
            {
                word_src->contents.direct_reg.src = word_src->contents.direct_reg.regnum;
            }
        }
    }
    /* Process the dest operand if one exists */
    if (success && op_dest != NULL)
    {
        word_dest = safe_malloc(sizeof(mc_word));
        success = process_operand(op_dest, props->op_dest_addr_rules, lineNumber, word_dest);
        if (success)
        {
            /* Update the dest operand addressing type */
            word_instruction->contents.instruction.dest_addressing = word_dest->type;

            /* If operand is direct reg, need to store the register number in dest reg */
            if (word_dest->type == WT_DIRECT_REG)
            {
                word_dest->contents.direct_reg.dest = word_dest->contents.direct_reg.regnum;
            }
        }
    }

    /* Debug log newline after printed the entire command */
    LOG("\n");

    if (success)
    {
        /* Now that we've parsed everything, time to write it to memory */
        /* First word is the instruction word. Always gets written */
        serialize_code_mc_word(word_instruction);

        /* There's a special case where we have both source and dest operands and they are both registers */
        if ((word_src != NULL) && (word_src->type == WT_DIRECT_REG) &&
            (word_dest != NULL && (word_dest->type == WT_DIRECT_REG)))
        {
            /* We store both registers in one word and so eliminate the word_src */
            word_dest->contents.direct_reg.src = word_src->contents.direct_reg.src;
            free_if_not_null(word_src);
        }

        /* Only if there's a source operand, write it to the code section */
        if (word_src != NULL)
        {
            serialize_code_mc_word(word_src);
        }

        /* Only if there's a dest operand, write it to the code section */
        if (word_dest != NULL)
        {
            serialize_code_mc_word(word_dest);
        }
    }

    /* Cleanup */
    free_if_not_null(op_src);
    free_if_not_null(op_dest);
    free_if_not_null(word_instruction);
    free_if_not_null(word_src);
    free_if_not_null(word_dest);

    return success;
}
