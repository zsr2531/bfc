cmake_minimum_required(VERSION 3.17)
project(bfc)

set(CMAKE_CXX_STANDARD 17)

add_executable(bfc main.cpp lexer.hpp lexer.cpp ast.hpp ast.cpp parser.hpp parser.cpp utils.hpp interpreter.hpp interpreter.cpp cli.hpp cli.cpp codegen.hpp codegen.cpp)