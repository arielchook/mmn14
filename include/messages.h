#ifndef MESSAGES_H
#define MESSAGES_H


#define ERR_USAGE "Usage: %s <file1> [<file2> .. <fileN>]\nWhere file1..fileN are .as files\n"
#define ERR_FILE_CANT_BE_READ "%s: file could not be opened for reading\n"
#define ERR_FILE_CANT_BE_WRITTEN "%s: cannot open file for writing\n"
#define ERR_MEM_ALLOC_FAILED "Memory allocation failed.\n"


/* Error messages for the pre-processing step */
#define PP_ERR_NO_NESTED_MACROS "Precomp:%d:Nested macros are not allowed\n"
#define PP_ERR_EXTRA_CHARS "Precomp:%d:Extraneous characters after macro name\n"
#define PP_ERR_INVALID_MACRO_NAME "Precomp:%d:invalid macro name specified\n"
#define PP_ERR_RESERVED_WORD "Precomp:%d:Cannot use reserved word as a macro name(%s)\n"
#define PP_ERR_EXTRA_ENDMCR "Precomp:%d:Extraneous characters after endmcr command\n"
#define PP_ERR_ENDMCR_MISLOCATION "Precomp:%d:endmcr encountered but not inside macro definition\n"

/* Error messages for first pass step */
#define ERR_MISSING_DEFINE "FirstPass:%d: Invalid .define statement. Expecting: .define name=val\n"
#define ERR_MISSING_EQUAL "FirstPass:%d: Invalid .define statement. Missing equal (=) sign\n"
#define ERR_MISSING_DEFINE_NAME "FirstPass:%d: Missing constant name in .define statement. Expecting: .define name=val\n"
#define ERR_DUP_SYMBOL "FirstPass:%d: Duplicate symbol definition (%s)\n"
#define ERR_RESERVED_WORD "FirstPass:%d: Cannot use a reserved word for symbol name\n"
#define ERR_INVALID_SYMBOL_NAME "FirstPass:%d: Symbol name must start with a letter and contain up to 31 alphanumeric letters\n"
#define ERR_MISSING_DEFINE_VALUE "FirstPass:%d: Constant value not specified. Expecting .define name=val\n"
#define ERR_DEFINE_VALUE_NOT_INT "FirstPass:%d: Value specified for constant must be an integer\n"
#define ERR_LABEL_WITH_NO_CMD "FirstPass:%d: Label specified with no command following it\n"
#define ERR_MISSING_VALUE "FirstPass:%d: Missing value in .data definition\n"
#define ERR_CANT_FIND_DEFINE "FirstPass:%d: Cannot find a constant with the name %s\n"
#define ERR_DATA_SECTION_FULL "FirstPass:%d: Data section is full. Cannot continue.\n"
#define ERR_MISSING_QUOTES "FirstPass:%d: .string value must be enclosed in quotes (\"\")\n"
#define ERR_CLOSING_FILE "Precomp:Error closing input file (%s%s)\n"
#define ERR_FOUND_IN_PRECOMP "Erros found in precompilation. Cannot continue...\n"
#define ERR_FOUND_IN_FIRSTPASS "Erros found in first-pass. Cannot continue...\n"
#define ERR_FOUND_IN_SECONDPASS "Erros found in second-pass. Cannot continue...\n"
#endif