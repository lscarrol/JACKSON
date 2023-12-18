# JACKSON

## Overview
This project is a compiler for a language called JACKSON. The compiler is written in C and uses a variety of data structures and algorithms to parse and generate code.

### Prerequisites
c99, bison, flex, smallpond

## Features
- Parses JACKSON source code into an Abstract Syntax Tree (AST).
- Generates Intermediate Representation (IR) code from the AST.
- Implements a Directed Acyclic Graph (DAG) for optimizing the IR code.
- Uses a hash table for storing and retrieving data.
- Uses a pair table for managing pairs of data.
- Uses a stack for managing the execution of the program.
- Uses a list of scopes for managing the visibility of variables.

## Components
1. **Parsing**
 - Uses a lexer to break the source code into tokens.
 - Uses a parser to generate an AST from the tokens.

2. **Code Generation**
 - Transforms the AST into IR code.
 - Optimizes the IR code using a DAG.

3. **Data Structures**
 - Uses a hash table for storing and retrieving data.
 - Uses a pair table for managing pairs of data.
 - Uses a stack for managing the execution of the program.
 - Uses a list of scopes for managing the visibility of variables.

## Technologies Used
- **C**: The primary programming language used for implementing the compiler.
- **Lex and Yacc**: Tools used for generating the lexer and parser.
- **Directed Acyclic Graph (DAG)**: Data structure used for optimizing the IR code.
- **Hash Table**: Data structure used for storing and retrieving data.
- **Pair Table**: Data structure used for managing pairs of data.
- **Stack**: Data structure used for managing the execution of the program.
- **Scopes**: Data structure used for managing the visibility of variables.

## Built With
* [flex](https://github.com/westes/flex) 
* [bison](https://www.gnu.org/software/bison/)
* [smallpond](https://github.com/ZacharySalim/Smallpond)
* [vim of course]

## Usage
- Compile the JACKSON source code into an executable using the provided makefile.
- Run the executable with the JACKSON source code as input.
make compiler
./compiler input.txt

## Contributors
- oswald

## License
Distributed under the MIT License. See `LICENSE` for more information.

