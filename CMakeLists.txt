cmake_minimum_required(VERSION 2.8)

project(simple_C_parser)

set(SOURCE_LIB parser.cpp tokenizator.cpp ast.cpp astnodes.cpp)
set(HEADERS parser.h tokenizator.h ast.h astnodes.h)
add_library(parser STATIC ${SOURCE_LIB})

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} parser)
