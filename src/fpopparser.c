/**
 * @file fpopparser.c
 * @brief Floating-Point Operand Parser.
 *
 * Implements functions for parsing the addressing type of operands in assembly statements.
 * This module provides functionality to analyze operands' syntax and determine their addressing type,
 * supporting immediate, direct, fixed index, and direct register addressing modes.
 *
 * The main functions in this file are:
 * - parse_op_addressing_type: Parses the addressing type of an operand based on given rules, considering the operand's syntax.
 * - count_operands_words: Counts the number of memory words needed for the operands of a machine code instruction,
 *   analyzing the operands and their addressing modes to determine the required memory allocation.
 *
 * These functions are essential for the translation of assembly statements to machine code instructions,
 * ensuring proper handling of operands and validation of addressing modes.
 *
 * The module also contains helper functions for string manipulation and error reporting,
 * facilitating the parsing and processing of assembly statements.
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
 * @brief Parses the addressing type of an operand based on given rules.
 *
 * This function determines the addressing type of the provided operand by analyzing its syntax.
 * It supports immediate, direct, fixed index, and direct register addressing modes.
 *
 * @param op The operand to parse.
 * @param address_rules The addressing rules that apply to this operand.
 * @param lineNumber The current line number in the source file for error reporting.
 * @return The addressing type enum value for the operand. Returns WT_INVALID if the addressing type is disallowed or malformed.
 */
enum addressing_type parse_op_addressing_type(char *op, uint8_t address_rules, int lineNumber)
{
    char *array_name, *array_index;
    bool improper_format;

    /* Make sure the operand is not empty */
    if (strlen(op) == 0)
    {
        printf(ERR_EMPTY_OPERAND, lineNumber);
        return WT_INVALID;
    }

    /* Immediate addressing */
    if (startsWith(op, IMMEDIATE_VALUE_PREFIX))
    {
        /* Is immediate addressing allowed for this operand? */
        if (!(address_rules & AR_IMDT))
        {
            printf(ERR_IMDT_DISALLOWED, lineNumber);
            return WT_INVALID;
        }
        return WT_IMMEDIATE;
    }

    /* Fixed index addressing */
    if (endsWith(op, ARRAY_CLOSE_CHAR))
    {
        /* Is fixed index addressing allowed for this operand? */
        if (!(address_rules & AR_FXIND))
        {
            printf(ERR_FXIND_DISALLOWED, lineNumber);
            return WT_INVALID;
        }

        /* Chop the ']' */
        op[strlen(op) - 1] = '\0';

        /* Extract the array name and the array index */
        array_name = extractWordSeparator(op, 1, NULL, ARRAY_OPEN_CHAR);
        array_index = extractWordSeparator(op, 2, NULL, ARRAY_OPEN_CHAR);
        /* If either doesn't exist, it's an improper format */
        improper_format = (array_name == NULL) || (array_index == NULL);

        /* Free memory from parsing */
        free_if_not_null(array_name);
        free_if_not_null(array_index);

        if (improper_format)
        {
            printf(ERR_MALFORMED_ARRAY, lineNumber);
            return WT_INVALID;
        }

        return WT_FIXED_INDEX; /* Fixed index addressing is 2 words - one for the label and another for the index */
    }

    /* Direct register addressing */
    if (is_register(op))
    {
        /* Is register addressing allowed for this operand? */
        if (!(address_rules & AR_DRREG))
        {
            printf(ERR_DRREG_DISALLOWED, lineNumber);
            return WT_INVALID;
        }
        return WT_DIRECT_REG;
    }

    /* If none of the above, it means it's direct addressing */
    /* Is direct addressing allowed for this operand? */
    if (!(address_rules & AR_DRCT))
    {
        printf(ERR_DIRECT_DISALLOWED, lineNumber);
        return WT_INVALID;
    }

    return WT_DIRECT;
}

/**
 * @brief Counts the number of memory words needed for the operands of a statement.
 *
 * Analyzes the operands of a machine code instruction to determine how many memory words
 * are required for them, considering the instruction properties and addressing modes.
 *
 * @param stmt The assembly statement containing the operands.
 * @param lineNumber The current line number in the source file for error reporting.
 * @param props The properties of the instruction, including allowed addressing modes.
 * @return True if the operands are valid and processed successfully, False otherwise.
 */
bool count_operands_words(char *stmt, int lineNumber, const instruction_props *props)
{
    bool success = true;
    enum addressing_type src_at = WT_INVALID, dest_at = WT_INVALID;
    int mem_words_count = 0;

    /* Get the first operand, second operand, and 3rd operand if such exists */
    char *op_src = extractWordSeparator(stmt, 1, NULL, OP_SEPARATOR);
    char *op_dest = extractWordSeparator(stmt, 2, NULL, OP_SEPARATOR);
    char *more_ops = extractWordSeparator(stmt, 3, NULL, OP_SEPARATOR);

    /* Check for incorrect number of operands:
     * 1. more than 2 operands or
     * 2. command has no operarnds and we have op_src set
     * 3. command has 1 operand and we have op_dest set or op_src is not set
     * 4. command has 2 operands and we don't have both op_src and op_dest set */
    if ((more_ops != NULL) ||
        ((props->num_operands == 0) && (op_src != NULL)) ||
        ((props->num_operands == 1) && ((op_dest != NULL) || (op_src == NULL))) ||
        ((props->num_operands == 2) && ((op_src == NULL) || (op_dest == NULL))))
    {
        printf(ERR_NUM_OPERANDS, lineNumber, props->instruction, props->num_operands);
        success = false;
    }

    /* One word needs to be counted for the instruction word */
    mem_words_count += 1;

    if (success)
    {
        /* If we have only 1 operand, it's the dest operand */
        if (props->num_operands == 1)
        {
            op_dest = op_src;
            op_src = NULL;
        }
    }

    /* Is there a source operand? */
    if (success && op_src != NULL)
    {
        src_at = parse_op_addressing_type(op_src, props->op_src_addr_rules, lineNumber);

        /* Advance the instruction counter (IC) by the number of words based on the command and the source operand */
        if ((success = (src_at != WT_INVALID)))
        {
            switch (src_at)
            {
            case WT_IMMEDIATE:
            case WT_DIRECT:
            case WT_DIRECT_REG:
                mem_words_count += 1;
                break;
            case WT_FIXED_INDEX:
                mem_words_count += 2;
                break;
            default:
                break;
            }
        }
    }

    /* Is there a dest operand? */
    if (success && op_dest != NULL)
    {
        dest_at = parse_op_addressing_type(op_dest, props->op_dest_addr_rules, lineNumber);

        if ((success = (dest_at != WT_INVALID)))
        {
            /* Advance the instruction counter (IC) by the number of words based on the command and the dest operand */
            switch (dest_at)
            {
            case WT_IMMEDIATE:
            case WT_DIRECT:
                mem_words_count += 1;
                break;
            /* Special case - if we have a src operand and it's direct reg we store both registers in the same word */
            case WT_DIRECT_REG:
                if (src_at != WT_DIRECT_REG)
                {
                    mem_words_count += 1;
                }
                break;
            case WT_FIXED_INDEX:
                mem_words_count += 2;
                break;
            default:
                break;
            }
        }
    }

    /* Now that we counted how many words the command will hold in machine code we should advance the
       instruction counter accordingly */
    advanceIC(mem_words_count);

    free_if_not_null(op_src);
    free_if_not_null(op_dest);
    free_if_not_null(more_ops);

    return success;
}
