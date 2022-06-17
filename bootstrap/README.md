bootstrap
=========

This directory is historic.

Originally, this Ruby code was used to 'bootstrap' the project. Initially, it was used to generate the bytecode the compiler itself uses to parse grammar files. It patched together the generated code with templates and generated some of the C source files. This bootstrap process was gradually replaced with alternatives, and is now completely unused.

The 'bootstrap' process is now streamlined in the top level Makefile as 'make bytecode'. This generates the compiler's bytecode .c and .h files from the grammar files located in the grammar directory.
