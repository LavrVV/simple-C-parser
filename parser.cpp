#include "parser.h"

AST parse(std::string s){
    auto tokens = tokenize(s);
    return AST(tokens);
}
