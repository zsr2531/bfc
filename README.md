# bfc
A brainfuck interpreter and compiler written in C++

## What does it do?

You can either give it a file or a raw string to interpret/compile. By `compile`, I mean that it can generate C code from any brainfuck program.

## Usage

`./bfc <-f|--file FILE>|<-e|--eval PROGRAM> [options]`

### Command line arguments

- `-h`, `--help`: Shows a help message.
- `--pretty-print-ast`: Pretty prints the AST.
- `-e`, `--eval`: Evaluates the provided brainfuck program.
- `-f`, `--file`: Evaluates a brainfuck program from a file.
- `-o`, `--output`: Dumps C pseudocode to a file.
