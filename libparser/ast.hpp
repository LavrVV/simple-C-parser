#ifndef AST_H
#define AST_H

#include <libparser/astnodes.hpp>

/** grammar:
 * parser:
 *   programm ::= statement* ;
 *   statement ::= if | for | while | assign | func_call | func_def | var_decl | return | ';' ;
 *   block ::= '{' statement* '}' ;
 *   if ::= 'if' '(' expr ')'  block ('else' block)? ;
 *   for ::= 'for' '(' statement ';' expr ';' statement ')' block ;
 *   while ::= 'while' '(' expr ')' block
 *   assign ::= T_NAME '=' expr ;
 *   func_call ::= // parse in expr
 *   func_def ::= T_TYPE T_NAME '(' T_TYPE T_NAME (',' T_TYPE T_NAME)* ')' block ;
 *   var_decl ::= T_TYPE (T_NAME '=' expr | T_NAME ';') ;
 *   return ::= 'return' expr ;
 *   expr ::= // TODO
 * lexer:
 *   T_TYPE ::= 'int' | 'float' | 'string' | 'char' | 'bool' | 'void' ;
 *   T_NAME ::= [a-zA-z] [a-zA-z0-9_]*
 *   T_OPERATOR ::= '*' | '/' | '+' | '-' | '!' | '||' | '&&'  ;
 * <literal> ::= true | false | <number> | '<char>' |  "string"
 * <operator> 
 * <val> ::= <literal> | <name>
 * <expression> ::= <val> <operator> <val> | <expression> <operator> <val> | ( <expression> ) | <val>
 * 
 */
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

    Context::Scope vars;
};

#endif