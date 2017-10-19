cmake_minimum_required(VERSION 2.6)

project(compiler)

set(CMAKE_CXX_FLAGS "-O2 -Wall -std=c++11")

add_executable(compiler
    src/Ast.cpp
    src/BasicBlockBuilder.cpp
    src/File.cpp
    src/Lexer.cpp
    src/main.cpp
    src/Parser.cpp
    src/Token.cpp
)
