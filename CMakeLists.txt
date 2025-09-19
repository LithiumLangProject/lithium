cmake_minimum_required(VERSION 3.20)
project(lithium)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lithium
        src/main.cpp
        src/lexar.hpp src/lexer.cpp
        src/parser.hpp src/parser.cpp
        src/ast.hpp src/ast.cpp
        src/semantic.hpp src/semantic.cpp
        src/codegen.hpp src/codegen.cpp
        src/types.hpp
        src/error.hpp src/error.cpp
        src/utils.hpp src/utils.cpp
)
