cmake_minimum_required(VERSION 3.13)

set(SOURCE_LIB parser.cpp tokenizator.cpp ast.cpp astnodes.cpp context.cpp base_astnode.cpp)

add_library(libparser STATIC "${SOURCE_LIB}")

target_compile_features(libparser PUBLIC cxx_std_17)