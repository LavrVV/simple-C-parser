# libparser
set(SRC_LIBPARSER 
    libparser/parser.cpp 
    libparser/tokenizator.cpp 
    libparser/ast.cpp 
    libparser/astnodes.cpp 
    libparser/context.cpp 
    libparser/base_astnode.cpp
)
add_library(libparser STATIC "${SRC_LIBPARSER}")
target_compile_features(libparser PUBLIC cxx_std_17)