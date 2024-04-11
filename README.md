# mmn14
OpenU Assembler
Overview
OpenU Assembler is a powerful assembly compiler developed by Ariel Cohen and Jonathan Transky. It's specifically designed to translate assembly language into an encrypted 4-base language format, catering to a unique machine architecture. The assembler simplifies macros, constructs an intricate symbol table, and ensures code security through an innovative encryption method.

Features
Macro Processing: Efficient handling and expansion of macros in the precompilation stage for streamlined assembly code.
Symbol Table Construction: Detailed first pass analysis to identify labels, directives, and instructions, creating a robust symbol table for reference resolution.
Instruction Processing: Second pass validation and translation of assembly instructions into an optimized and secure machine code.
Encryption to 4-Base Language: Unique binary-to-4-base encryption using specific operators for enhanced code security.
Workflow
The compilation process of OpenU Assembler is divided into three main stages:

Precompilation: Macro definitions and calls are simplified, enhancing the clarity and manageability of the assembly code.
First Pass: The assembler scans the assembly code to process directives, labels, and instructions, setting up a detailed symbol table.
Second Pass: Final processing includes validation of .entry directives and conversion of instructions into encrypted machine code.
Getting Started
To use the OpenU Assembler, follow this basic command-line syntax:


openu_assembler input_file.asm -o output_file


Building OpenU Assembler

# Compile the project
make


License
OpenU Assembler is released under the MIT License. For more information, see the LICENSE file.

Acknowledgments
Special thanks to Ariel Cohen and Jonathan Transky for their dedication and hard work in building OpenU Assembler, making it a valuable tool for assembly language compilation and encryption.

