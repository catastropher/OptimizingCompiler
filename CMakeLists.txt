cmake_minimum_required(VERSION 2.6)

project(compiler)

set(CMAKE_CXX_FLAGS "-O2 -Wall")

add_executable(compiler
    src/Lexer.cpp
	src/main.cpp
)
