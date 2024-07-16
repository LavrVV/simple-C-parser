#ifndef TOKENIZATOR_H
#define TOKENIZATOR_H

#include <string>
#include <vector>

enum class TokenType {
    operator_token,
    name_token,
    type_token,
    open_bracket_token,
    close_bracket_token,
    literal_token
};

class Token {
public:
    Token(const std::string& s, TokenType t);
    Token(const std::string& s, TokenType t, int p);

    TokenType token_type;

    int priority;
    
    const std::string& get_value() const;
private:
    std::string token;   
};

std::vector<Token> tokenize(const std::string& s);

#endif
