#include <libparser/parser.hpp>

AST parse(std::string s){
    auto tokens = tokenize(s);
    return AST(tokens);
}
