
#include <fpopparser.h>
#include <reserved.h>
#include <messages.h>
#include <utils.h>
#include <symbols.h>
#include <mcwordtypes.h>
#include <machinecode.h>

enum addressing_type parse_op_addressing_type(char *op, uint8_t address_rules, int lineNumber)
{
    char *array_name, *array_index;

    /* make sure it's not empty */
    ltrim(op);
    if (strlen(op) == 0)
    {
        printf(ERR_EMPTY_OPERAND, lineNumber);
    }
    rtrim(op);

    /* immediate addressing */
    if (startsWith(op, IMMEDIATE_VALUE_PREFIX))
    {
        /* is immediate addressing allowed for this operand? */
        if (!(address_rules & AR_IMDT))
        {
            printf(ERR_IMDT_DISALLOWED, lineNumber);
            return WT_INVALID;
        }
        return WT_IMMEDIATE;
    }

    /* fixed index addressing */
    if (endsWith(op, ARRAY_CLOSE_CHAR))
    {
        /* is fixed index addressing allowed for this operand? */
        if (!(address_rules & AR_FXIND))
        {
            printf(ERR_FXIND_DISALLOWED, lineNumber);
            return WT_INVALID;
        }

        /* chop the ']' */
        op[strlen(op) - 1] = '\0';

        /* extract the array name and the array index */
        if (((array_name = extractWordSeparator(op, 1, NULL, ARRAY_OPEN_CHAR)) == NULL) ||
            ((array_index = extractWordSeparator(op, 2, NULL, ARRAY_OPEN_CHAR)) == NULL))
        {
            printf(ERR_MALFORMED_ARRAY, lineNumber);
            return WT_INVALID;
        }

        return WT_FIXED_INDEX; /* fixed index addressing is 2 words - one for the label and another for the index */
    }

    /* direct register addressing */
    if (is_register(op))
    {
        /* is register addressing allowed for this operand? */
        if (!(address_rules & AR_DRREG))
        {
            printf(ERR_DRREG_DISALLOWED, lineNumber);
            return WT_INVALID;
        }
        return WT_DIRECT_REG;
    }

    /* if none of the above it means it's direct addressing */

    /* is direct addressing allowed for this operand? */
    if (!(address_rules & AR_DRCT))
    {
        printf(ERR_DIRECT_DISALLOWED, lineNumber);
        return WT_INVALID;
    }

    return WT_DIRECT;
}

bool count_operands_words(char *stmt, int lineNumber, const instruction_props *props)
{
    bool success = true;
    enum addressing_type src_at, dest_at;

    /* get the first operand, second operand and 3rd operand if such */
    char *op_src = extractWordSeparator(stmt, 1, NULL, OP_SEPARATOR);
    char *op_dest = extractWordSeparator(stmt, 2, NULL, OP_SEPARATOR);
    char *more_ops = extractWordSeparator(stmt, 3, NULL, OP_SEPARATOR);

    /* if any of the following applies we have more operands than we should:
    1. more than 2 operands or
    2. required number of operands for the command is 1 and there are 2 or
    3. required number of operands for the command is 0 and there is more than 0 */
    if ((more_ops != NULL) || ((op_dest != NULL) && (props->num_operands < 2)) || ((op_src != NULL) && (props->num_operands < 1)))
    {
        printf(ERR_NUM_OPERANDS, lineNumber, props->instruction);
        success = false;
    }

    if (success)
    {
        /* if we have only 1 operand, it's the dest operand */
        if (props->num_operands == 1)
        {
            op_dest = op_src;
            op_src = NULL;
        }
    }

    /* is there a source operand ? */
    if (success && op_src != NULL)
    {
        src_at = parse_op_addressing_type(op_src, props->op_src_addr_rules, lineNumber);
        /* advance the instruction counter (IC) by the number of words based on the command and the source opreand */
        if ((success = (src_at != WT_INVALID)))
        {
            switch (src_at)
            {
            case WT_IMMEDIATE:
            case WT_DIRECT:
            case WT_DIRECT_REG:
                IC += 1;
                break;
            case WT_FIXED_INDEX:
                IC += 2;
                break;
            default:
                break;
            }
        }
    }

    /* is there a dest operand ? */
    if (success && op_dest != NULL)
    {
        dest_at = parse_op_addressing_type(op_dest, props->op_dest_addr_rules, lineNumber);
        if ((success = (dest_at != WT_INVALID)))
        {
            /* advance the instruction counter (IC) by the number of words based on the command and the dest opreand */
            switch (dest_at)
            {
            case WT_IMMEDIATE:
            case WT_DIRECT:
                IC += 1;
                break;
            /* special case - if we have a src operand and it's direct reg we store both registers in the same word */
            case WT_DIRECT_REG:
                if (src_at != WT_DIRECT_REG)
                {
                    IC += 1;
                }
                break;
            case WT_FIXED_INDEX:
                IC += 2;
                break;
            default:
                break;
            }
        }
    }

    return success;
}