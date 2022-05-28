#include <iostream>
#include <libparser/parser.hpp>

int main(int argc, char** argv) {
    if(argc > 0){
        AST a = parse(argv[0]);
        a.execute();
    } else {
        AST a = parse("");
        a.execute();
    }
    return 0;
}
