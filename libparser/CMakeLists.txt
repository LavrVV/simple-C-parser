cmake_minimum_required(VERSION 3.13)

set(SOURCE_LIB parser.cpp tokenizator.cpp ast.cpp astnodes.cpp)

add_library(libparser STATIC "${SOURCE_LIB}")