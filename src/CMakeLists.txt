# libparser
set(SRC_LIBPARSER 
    libparser/parser.cpp 
    libparser/tokenizator.cpp 
    libparser/ast.cpp 
    libparser/astnodes.cpp 
    libparser/context.cpp 
    libparser/base_astnode.cpp
)
add_library(parser STATIC "${SRC_LIBPARSER}")
target_compile_features(parser PUBLIC cxx_std_17)
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(parser PRIVATE -Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCXX)
