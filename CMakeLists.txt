cmake_minimum_required(VERSION 3.13)

project(simple_C_parser VERSION 0.1.0)

include_directories(.)
#executable
add_subdirectory(simple_C_parser)
#libs
add_subdirectory(libparser)

target_link_libraries(${PROJECT_NAME} libparser)
