/**
 * @file mcwordtypes.h
 * @brief Machine Code Word Types and Structures.
 *
 * Defines enums and structures for representing various types of machine
 * code words and their components in an assembly language compiler or assembler.
 * This includes enumerating addressing types, defining a union for different
 * memory word formats, and structuring a complete machine code word. These
 * definitions are essential for encoding assembly instructions and data into
 * binary machine code format, supporting a range of instruction types and
 * addressing modes.
 */



#ifndef MCWORDTYPES_H
#define MCWORDTYPES_H

#include <stdio.h>
#include <stdint.h>

/**
 * Enumerates the supported addressing types, which also indicate the type of memory word in the mc_word union.
 */
enum addressing_type
{
    /** Immediate addressing type. */
    WT_IMMEDIATE = 0,  
    /** Direct addressing type. */
    WT_DIRECT = 1,     
    /** Fixed index addressing type, spanned over two memory words. */
    WT_FIXED_INDEX = 2,
    /** Direct register addressing type. */
    WT_DIRECT_REG = 3, 
    /** Represents a full instruction. */
    WT_INSTRUCTION = 4,
    /** Indicates an invalid or undefined addressing type. */
    WT_INVALID         
};

/**
 * Enumerates the symbols for absolute, relocatable, and external addressing, used in A_R_E fields.
 */
enum abs_rloc_extern
{
    /** Absolute addressing. */
    ARE_ABS = 0,     
    /** External addressing, for symbols defined in other modules. */
    ARE_EXTERN = 1,  
    /** Relocatable addressing, for symbols that can move in memory. */
    ARE_RELOC = 2    
};

/**
 * Defines a union representing a memory word in the machine code, accommodating different instruction and addressing formats.
 */
typedef union
{
    /**
     * Represents the first word of an instruction in memory, including opcode and addressing types.
     */
    struct
    {
        /** 2-bit Absolute, Relocatable, or External (A_R_E) indicator. */
        uint8_t A_R_E;           
        /** 2-bit destination operand addressing type. */
        uint8_t dest_addressing; 
        /** 2-bit source operand addressing type. */
        uint8_t src_addressing;  
        /** 4-bit opcode of the instruction. */
        uint8_t opcode;          
    } instruction;

    /**
     * Represents an immediate addressing memory word.
     */
    struct
    {
        /** 2-bit A_R_E indicator. */
        uint8_t A_R_E;  
        /** 12-bit immediate value. */
        uint16_t value; 
    } immediate;

    /**
     * Represents a direct addressing memory word, potentially referring to an external symbol.
     */
    struct
    {
        /** 2-bit A_R_E indicator. */
        uint8_t A_R_E;    
        /** 12-bit address of the label. */
        uint16_t address; 
        /** Pointer to an external symbol, if any. */
        char *external_symbol;
    } direct;

    /**
     * Represents a fixed index addressing spanned over two memory words, potentially referring to an external symbol.
     */
    struct
    {
        /** 2-bit A_R_E for the first word. */
        uint8_t A_R_E_1;         
        /** 12-bit address of the array. */
        uint16_t array;          
        /** 2-bit A_R_E for the second word. */
        uint8_t A_R_E_2;         
        /** 12-bit array index. */
        uint16_t index;          
        /** Pointer to an external symbol, if any. */
        char *external_symbol;   
    } fixed_index;

    /**
     * Represents a direct register addressing memory word.
     */
    struct
    {
        /** 2-bit A_R_E indicator. */
        uint8_t A_R_E; 
        /** 3-bit destination register number. */
        uint8_t dest;  
        /** 3-bit source register number. */
        uint8_t src;   
        /** Additional register number, if used. */
        uint8_t regnum;
    } direct_reg;

} mc_word_union;

/**
 * Struct that encapsulates a memory word in the machine code, including its type and content.
 */
typedef struct
{
    /** The type of memory word, determining which fields in the union are relevant. */
    enum addressing_type type; 
    /** The content of the memory word, as defined by the union. */
    mc_word_union contents;    
} mc_word;

#endif /* MCWORDTYPES_H */
