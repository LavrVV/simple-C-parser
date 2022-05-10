#ifndef AST_H
#define AST_H

#include <libparser/astnodes.hpp>

class AST {
public:
    explicit AST(std::vector<Token> tokens);

    std::string execute();

private:
    void parse(std::vector<Token>& tokens);

    std::shared_ptr<ASTNode> parse_statment(std::vector<Token>& tokens, size_t& pos);

    std::shared_ptr<ASTNode> parse_expression(std::vector<Token>& tokens, size_t& pos);

    std::shared_ptr<ASTNode> parse_block(std::vector<Token>& tokens, size_t& pos);

    std::shared_ptr<ASTNode> parse_function_call(std::vector<Token>& tokens, size_t& pos);

    std::shared_ptr<ASTNode> node_from_token(const Token& token);

    void build_expression_ast(std::stack<std::shared_ptr<ASTNode>>& out_stack, std::shared_ptr<ASTNode> curr);

    std::vector<std::shared_ptr<ASTNode>> root;

    std::map<std::string, std::shared_ptr<ASTNode>> vars;
};

#endif
