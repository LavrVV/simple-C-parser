cmake_minimum_required(VERSION 3.13)

project(simple_C_parser VERSION 0.1.0)

include_directories(include)
#executable
add_subdirectory(${PROJECT_NAME})
#libs
add_subdirectory(src)
#tests
add_subdirectory(tests)

target_link_libraries(${PROJECT_NAME} libparser)
