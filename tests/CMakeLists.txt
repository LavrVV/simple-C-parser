# tests for libparser
set(SRC_LIBPARSER_TESTS 
    libparser/context.test.cpp 
    libparser/tokenizator.test.cpp 
    libparser/parser.test.cpp
)
add_executable(libparser_test ${SRC_LIBPARSER_TESTS})
target_compile_features(libparser_test PUBLIC cxx_std_17)
target_link_libraries(libparser_test PRIVATE Catch2::Catch2WithMain parser)
# add to CTest
catch_discover_tests(libparser_test)