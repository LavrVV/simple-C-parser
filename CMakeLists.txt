cmake_minimum_required(VERSION 3.15)

project(simple_C_parser VERSION 0.1.0)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    include(cmake/LoadCLI.cmake)
    include(cmake/LoadCatch2.cmake)
    include(CTest)
endif()

# headers
include_directories(include)
# libs
add_subdirectory(src)
# executable
add_subdirectory(apps)
# tests
add_subdirectory(tests)

# install
if(WIN32)
    install(TARGETS ${PROJECT_NAME} RUNTIME)
    include(CPack)
endif()