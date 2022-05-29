#include <iostream>
#include <libparser/parser.hpp>

int main(int argc, char** argv) {
    if (argc > 1) {
        AST a = parse(argv[1]);
        a.execute();
    } else {
        AST a = parse("");
        a.execute();
    }
    return 0;
}
