#include <libparser/tokenizator.hpp>

#include <cctype>


Token::Token(std::string s, TokenType t): token(s), token_type(t), priority(0) {}

Token::Token(std::string s, TokenType t, int p): token(s), token_type(t), priority(p) {}

std::string Token::get_value() const {
    return  this->token;
}


bool is_open_bracket(char c) {
    if (c == '[' || c == '{' || c == '(' || c == '<')
        return true;
    else
        return false;
}

bool is_close_bracket(char c) {
    if (c == ']' || c == '}' || c == ')' || c == '>')
        return true;
    else
        return false;
}

// TODO add > < >= <= operator
bool is_operator(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '!' ||
            c == '|' || c == '&' || c == '=' || c == ',' || c == ';')
        return true;
    else
        return false;
}


bool is_separator(char c) {
    if (is_operator(c) || is_open_bracket(c) || is_close_bracket(c) || isspace(c))
        return true;
    else
        return false;
}


Token read_operator(std::string& s, size_t& from) {
    std::string res(1, s[from]);
    if (is_operator(s[from + 1])) {
        res += s[from + 1];
        from++;
    }
    auto t = Token(res, TokenType::operator_token);
    // set priority
    if (res == "*" or res == "/" or res == "%")
        t.priority = 2;
    else if(res == "+" or res == "-")
        t.priority = 1;
    return t;
}

// can be read literal or operator
Token read_name(std::string& s, size_t& from) {
    std::string res = "";
    for (;from < s.size(); ++from) {
        if (isspace(s[from]) or 
            is_operator(s[from]) or 
            is_open_bracket(s[from]) or 
            is_close_bracket(s[from])) {
            from--;
            break;
        } else {
            res += s[from];
        }
    }
    auto t = TokenType::name_token;
    if (res == "false" or res == "true") {
        t = TokenType::literal_token;
    } else if (res == "return" or res == "for" or 
               res == "if" or res == "else" or 
               res == "while") {
        t = TokenType::operator_token;
    } else if (res == "int" or res == "float" or res == "string" or
               res == "char" or res == "bool" or res == "void") {
        t = TokenType::type_token;
    }
    return Token(res, t);
}


Token read_literal(std::string& s, size_t& from) {
    std::string res("");
    if (s[from] == '\"') {  // string
        res += s[from];
        from++;
        while (s[from] != '\"') {
            res += s[from];
            from++;
        }
        res += s[from];
    } else if (s[from] == '\'' and s[from + 2] == '\'') {  // char
        res += std::string("\'") + s[from + 1] + "\'";
        from += 2;
    } else if (isdigit(s[from])) {  // number
        bool point = false;
        while (isdigit(s[from]) or (s[from] == '.' and !point)) {
            if (s[from] == '.')
                point = true;
            res += s[from];
            from++;
        }
        from--;
    }
    return Token(res, TokenType::literal_token);
}

std::vector<Token> tokenize(std::string& s) {
    auto res = std::vector<Token>();
    for (size_t i = 0; i < s.size(); ++i) {
        if (is_operator(s[i])) {
            res.push_back(read_operator(s, i));
        } else if (isalpha(static_cast<unsigned char>(s[i]))) {
            res.push_back(read_name(s, i));
        } else if (isdigit(s[i]) or s[i] == '\"' or s[i] == '\'') {
            res.push_back(read_literal(s, i));
        } else if (is_open_bracket(s[i])) {
            res.push_back(Token(std::string(1, s[i]), TokenType::open_bracket_token));
        } else if (is_close_bracket(s[i])) {
            res.push_back(Token(std::string(1, s[i]), TokenType::close_bracket_token));
        }
    }
    return res;
}
