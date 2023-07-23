#include <iostream>
#include <fstream>
#include <libparser/parser.hpp>
#include <CLI/CLI.hpp>

std::string read_file(std::ifstream& file) {
    std::string tmp;
    std::string res("");
    while (std::getline(file, tmp)) {
        res += tmp;
    }
    return res;
}

int main(int argc, char** argv) {
    CLI::App app{"simple C parser"};
    
    std::string file("");
    app.add_option("-f,--file", file, "script file to run");
    CLI11_PARSE(app, argc, argv);

    std::ifstream fscript;
    fscript.open(file);
    if (fscript.is_open()) {
        std::string script = read_file(fscript);
        AST a = parse(script);
        a.execute();
    } else {
        std::cerr << "file not found: " << file << std::endl;
        return 1;
    }
    return 0;
}
