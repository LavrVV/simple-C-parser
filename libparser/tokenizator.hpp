#ifndef TOKENIZATOR_H
#define TOKENIZATOR_H

#include <string>
#include <vector>

enum TokenType {
    operator_token,
    name_token,
    type_token,
    open_bracket_token,
    close_bracket_token,
    literal_token
};

class Token {
public:
    TokenType token_type;
    Token(std::string s, TokenType t);
    std::string get_value() const;
    int priority;
private:
    std::string token;   
};

std::vector<Token> tokenize(std::string s);

#endif
