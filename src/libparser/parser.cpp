#include <libparser/parser.hpp>

AST parse(const std::string& s) {
    auto tokens = tokenize(s);
    return AST(tokens);
}
