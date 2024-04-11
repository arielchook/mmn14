# Maman14 - OpenU Assembler

Authors: **Ariel Cohen** & **Jonathan Transky**

## Overview

OpenU Assembler is a powerful assembly compiler developed by Ariel Cohen and Jonathan Transky. It's specifically designed to translate assembly language into an encrypted 4-base language format, catering to a unique machine architecture. The assembler simplifies macros, constructs an intricate symbol table, and ensures code security through an innovative encryption method.

### Features

Macro Processing: Efficient handling and expansion of macros in the precompilation stage for streamlined assembly code.
Symbol Table Construction: Detailed first pass analysis to identify labels, directives, and instructions, creating a robust symbol table for reference resolution.
Instruction Processing: Second pass validation and translation of assembly instructions into an optimized and secure machine code.
Encryption to 4-Base Language: Unique binary-to-4-base encryption using specific operators for enhanced code security.

### Workflow

The compilation process of OpenU Assembler is divided into three main stages:

1. Precompilation: Macro definitions and calls are simplified, enhancing the clarity and manageability of the assembly code.
2. First Pass: The assembler scans the assembly code to process directives, labels, and instructions, setting up a detailed symbol table.
3. Second Pass: Final processing includes validation of .entry directives and conversion of instructions into encrypted machine code.

## Building

To build the project, simply run `make` in the project's root directory.  
This will generate the `assembler` executable in the `bin` folder.

## Usage

To use the compiler, run `./bin/assembler` followed by names of the input files with no extension.  
The assembler accepts multiple files and processes them one by one.
For example:
`./bin/assembler test/ps` will run the assembler on the file `ps.as` under the `test` folder.

At the end of a successful assembler compilation the following files will be generated:

- `test/ps.am` - an assembly file with all the macros that exist in `test/ps.as` expanded
- `test/ps.ob` - an object file containing the compiled code section and data section
- `test/ps.ent` - a file containing a list of entries (i.e. symbols that are available for other programs to refer to)
- `test/ps.ext` - a file containing a list of external symbols in `test/ps.as` and where in the code section they are referred to.

## Testing

To run a test suite of over 20 use cases run:  
`make test`  
This will run the assembler on all `.as` files in the `test` directory.  
Each test file has a comment in the first line describing what's being tested. All the test files check for error cases.  
It also runs the assembler on the ps.as file that was given in the guidelines and compares the results to the expected output files that
reside under the `test/forum_files` folder.

In the `Makefile` there's an option to include a **DEBUG** symbol in the gcc command line and enable enhanced debug logging.
