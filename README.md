# MINI_c_comolier

**Mini C Compiler**
A lightweight, self-contained C compiler written in C. It supports a subset of the C language, including variables, arithmetic, if/else conditionals, while loops, functions, and a built-in printf function.

The compiler performs the following stages:

Lexical Analysis (Lexer): Tokenizes the source code.

Parsing (Parser): Builds an Abstract Syntax Tree (AST).

Semantic Analysis: Checks for basic errors (variable declarations).

Intermediate Code Generation (ICG): Converts AST to 3-Address Code (Quadruples).

Code Generation & VM: Converts Quadruples to Bytecode and executes them on a custom stack-based Virtual Machine.

**Features**

Data Types: int (variables and literals).

Control Flow: if, else, while.

Arithmetic: +, -, *, /.

Comparisons: ==, !=, <, >.

Functions: Function definitions, calls, and return statements.

I/O: printf support (handles string literals and integer arguments like printf("Value: %d", x);).

Comments: Support for single-line comments //.

**File Structure**

main.c: Entry point. Orchestrates the compilation phases and runs the VM.

lexer.c: Reads the source file and converts characters into tokens (keywords, IDs, numbers).

parser.c: Analyzes the token stream and builds the AST nodes.

semantic.c: Traverses the AST to check for basic semantic correctness.

icg.c: Generates Intermediate Code (Quadruples) from the AST.

codegen.c: The "Backend". Contains the Virtual Machine (VM) that executes the generated code.

symbol_table.c: Manages variable names and memory offsets.

mini_c.h: Shared header file containing struct definitions and function prototypes.

Makefile: Build script.

Building the Compiler

You can compile the project using gcc.

**Option 1: Using GCC directly**

gcc -o mini_c main.c lexer.c parser.c semantic.c icg.c codegen.c symbol_table.c


**Option 2: Using Make (if installed)**

make


Running the Compiler

**Create a C source file (e.g., test.c) and run the compiler against it.**
./mini_c test.c


Example test.c

int main() {
    int i = 0;
    while(i < 3) {
        printf("Loop count: %d", i);
        i = i + 1;
    }

    int a = 10;
    if (a > 5) {
        printf("A is big: %d", a);
    } else {
        printf("A is small");
    }
    
    return 0;
}


Mini C Compiler

A lightweight, self-contained C compiler written in C. It supports a subset of the C language, including variables, arithmetic, if/else conditionals, while loops, functions, and a built-in printf function.

The compiler performs the following stages:

Lexical Analysis (Lexer): Tokenizes the source code.

Parsing (Parser): Builds an Abstract Syntax Tree (AST).

Semantic Analysis: Checks for basic errors (variable declarations).

Intermediate Code Generation (ICG): Converts AST to 3-Address Code (Quadruples).

Code Generation & VM: Converts Quadruples to Bytecode and executes them on a custom stack-based Virtual Machine.

Features

Data Types: int (variables and literals).

Control Flow: if, else, while.

Arithmetic: +, -, *, /.

Comparisons: ==, !=, <, >.

Functions: Function definitions, calls, and return statements.

I/O: printf support (handles string literals and integer arguments like printf("Value: %d", x);).

Comments: Support for single-line comments //.

File Structure

main.c: Entry point. Orchestrates the compilation phases and runs the VM.

lexer.c: Reads the source file and converts characters into tokens (keywords, IDs, numbers).

parser.c: Analyzes the token stream and builds the AST nodes.

semantic.c: Traverses the AST to check for basic semantic correctness.

icg.c: Generates Intermediate Code (Quadruples) from the AST.

codegen.c: The "Backend". Contains the Virtual Machine (VM) that executes the generated code.

symbol_table.c: Manages variable names and memory offsets.

mini_c.h: Shared header file containing struct definitions and function prototypes.

Makefile: Build script.

Building the Compiler

You can compile the project using gcc.

Option 1: Using GCC directly

gcc -o mini_c main.c lexer.c parser.c semantic.c icg.c codegen.c symbol_table.c


Option 2: Using Make (if installed)

make


Running the Compiler

Create a C source file (e.g., test.c) and run the compiler against it.

./mini_c test.c


Example test.c

int main() {
    int i = 0;
    while(i < 3) {
        printf("Loop count: %d", i);
        i = i + 1;
    }

    int a = 10;
    if (a > 5) {
        printf("A is big: %d", a);
    } else {
        printf("A is small");
    }
    
    return 0;
}


**Expected Output:**

[1/5] Lexing complete.
[2/5] Parsing...
[3/5] Semantic Analysis...
[4/5] Intermediate Code Generation...
[5/5] Running VM...

--- Generating & Executing VM Code ---
--- Output ---
Loop count: 0
Loop count: 1
Loop count: 2
A is big: 10


**Troubleshooting**

"Error: Cannot open file": Ensure the test.c file exists in the same directory.

"Expected token..." errors: Check your syntax. This compiler is strict:

Use int for all functions (e.g., int main(), not void main()).

Ensure semicolons ; are present.

Variable declarations must include initialization (e.g., int x = 0;).

[1/5] Lexing complete.
[2/5] Parsing...
[3/5] Semantic Analysis...
[4/5] Intermediate Code Generation...
[5/5] Running VM...

--- Generating & Executing VM Code ---
--- Output ---
Loop count: 0
Loop count: 1
Loop count: 2
A is big: 10


Troubleshooting

"Error: Cannot open file": Ensure the test.c file exists in the same directory.

"Expected token..." errors: Check your syntax. This compiler is strict:

Use int for all functions (e.g., int main(), not void main()).

Ensure semicolons ; are present.

Variable declarations must include initialization (e.g., int x = 0;).
