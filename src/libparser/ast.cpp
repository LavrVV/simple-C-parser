#include <libparser/ast.hpp>

#include <queue>


AST::AST(const std::vector<Token>& tokens) {
    this->parse(tokens);
}

const std::vector<std::shared_ptr<ASTNode>>& AST::get_root() const {
    return root;
}

std::string AST::execute() {
    Value res("");
    Context context;
    context.push_scope(&vars);
    for (const auto& st: root)
        res = st->execute(context);
    return res.get_value();
}

void AST::parse(const std::vector<Token>& tokens) {
    size_t pos = 0;
    while (pos < tokens.size()) {
        auto st = this->parse_statment(tokens, pos);
        root.push_back(st);
    }
    std::vector<std::shared_ptr<ASTNode>> main;
    auto f = new CallFunction(this->vars["main"], main);
    root.push_back(std::shared_ptr<ASTNode>(f));
}


std::shared_ptr<ASTNode> AST::parse_statment(const std::vector<Token>& tokens, size_t& pos) {
    if (tokens[pos].get_value() == ";") {
        pos++;
        return std::shared_ptr<ASTNode>(new ASTNode());
    } else if (tokens[pos].token_type == TokenType::name_token) {
        if (tokens[pos + 1].get_value() == "(") {
            // call function
            return this->parse_expression(tokens, pos);
        } else if (tokens[pos + 1].get_value() == "=") {
            // assigment
            auto name = tokens[pos].get_value();
            pos += 2;
            auto res = this->parse_expression(tokens, pos);
            auto assign = new Assign(name, res);
            return std::shared_ptr<ASTNode>(assign);
        }
    } else if (tokens[pos].token_type == TokenType::type_token) {
        if (tokens[pos + 2].get_value() == "(") {
            // function definition
            auto ret_type = tokens[pos].get_value();
            pos++;
            auto name = tokens[pos].get_value();
            pos += 2;
            std::vector<std::string> params; // names
            for (; pos < tokens.size(); pos += 3) {
                if (tokens[pos].token_type == TokenType::close_bracket_token) {
                    break;
                } else {
                    params.push_back(tokens[pos + 1].get_value());
                }
            }
            pos += 2;
            auto body = this->parse_block(tokens, pos);
            auto foo = new Function(name, ret_type, params, body);
            auto res = std::shared_ptr<ASTNode>(foo);
            this->vars[name] = res;
            return res;
        } else {
            // var decl
            auto var_type = tokens[pos].get_value();
            auto var_name = tokens[pos + 1].get_value();
            auto decl = new VariableDeclaration(var_name, var_type);
            if (tokens[pos + 2].get_value() == "=") {
                pos += 1;
            } else {
                pos += 3;
            }
            return std::shared_ptr<ASTNode>(decl);
        }
    } else if (tokens[pos].token_type == TokenType::operator_token) {
        if (tokens[pos].get_value() == "for") {
            pos += 2;
            auto decl = this->parse_statment(tokens, pos);
            auto def = this->parse_statment(tokens, pos);
            auto condition = this->parse_expression(tokens, pos);
            auto incr = this->parse_statment(tokens, pos);
            pos++;
            auto block = this->parse_block(tokens, pos);
            auto resfor = new For(decl, def, condition, incr, block);
            return std::shared_ptr<ASTNode>(resfor);
        } else if (tokens[pos].get_value() == "while") {
            pos += 2;
            auto condition = this->parse_expression(tokens, pos);
            pos += 1;
            auto block = this->parse_block(tokens, pos);
            auto reswhile = new While(condition, block);
            return std::shared_ptr<ASTNode>(reswhile);
        } else if (tokens[pos].get_value() == "if") { // TODO support without {}
            pos += 2;
            auto condition = this->parse_expression(tokens, pos);
            pos += 1;
            auto block = this->parse_block(tokens, pos);
            //else
            If* resif;
            if (tokens[pos].get_value() == "else") {
                pos += 2;
                auto else_block = this->parse_block(tokens, pos);
                resif = new If(condition, block, else_block);
            } else {
                resif = new If(condition, block);
            }
            return std::shared_ptr<ASTNode>(resif);
        } else if (tokens[pos].get_value() == "return") {
            pos++;
            auto expr = this->parse_expression(tokens, pos);
            auto ret = new Return(expr);
            return std::shared_ptr<ASTNode>(ret);
        }
    }
    return std::shared_ptr<ASTNode>(new ASTNode());
}

std::shared_ptr<ASTNode> AST::parse_expression(const std::vector<Token>& tokens, size_t& pos) {
    std::stack<Token> temp;
    std::stack<std::shared_ptr<ASTNode>> out_stack;
    //create output stack of tokens
    int brackets = 0;
    for (; pos < tokens.size(); ++pos) {
        if (tokens[pos].get_value() == ";" || tokens[pos].get_value() == ",") {
            pos++;
            break;
        } else if (tokens[pos].token_type == TokenType::literal_token) {
            out_stack.push(node_from_token(tokens[pos]));
        }
        else if (tokens[pos].token_type == TokenType::operator_token) {
            while (!temp.empty() && temp.top().priority >= tokens[pos].priority) {
                out_stack.push(node_from_token(temp.top()));
                temp.pop();
            }
            temp.push(tokens[pos]);
        } else if (tokens[pos].token_type == TokenType::name_token) {
            if (pos < tokens.size() - 1 && tokens[pos + 1].token_type == TokenType::open_bracket_token) {
                out_stack.push(parse_function_call(tokens, pos));
            } else {
                out_stack.push(node_from_token(tokens[pos]));
            }
        } else if (tokens[pos].token_type == TokenType::open_bracket_token) {
            brackets++;
            temp.push(tokens[pos]);
        } else if (tokens[pos].token_type == TokenType::close_bracket_token) {
            brackets--;
            if (brackets < 0) {
                break;
            }
            while (temp.top().token_type != TokenType::open_bracket_token) {

                out_stack.push(node_from_token(temp.top()));
                temp.pop();
            }
            if (temp.top().token_type == TokenType::open_bracket_token) {
                temp.pop();
            }
        }
    }
    while (!temp.empty()) {
        //brackets
        out_stack.push(node_from_token(temp.top()));
        temp.pop();
    }
    std::shared_ptr<ASTNode> root = out_stack.top();
    out_stack.pop();
    if (std::dynamic_pointer_cast<Operator>(root)) {
        build_expression_ast(out_stack, root);
    }
    return root;
}

std::shared_ptr<ASTNode> AST::parse_block(const std::vector<Token>& tokens, size_t& pos) {
    Block* root = new Block();
    if (tokens[pos].get_value() == "{") pos++;
    for (; pos < tokens.size(); ) {
        if (tokens[pos].get_value() == "}") {
            break;
        } else {
            root->add_child(this->parse_statment(tokens, pos));
        }
    }
    pos++;
    return std::shared_ptr<ASTNode>(root);
}

std::shared_ptr<ASTNode> AST::parse_function_call(const std::vector<Token>& tokens, size_t& pos) {
    std::vector<std::shared_ptr<ASTNode>> params;
    auto root = this->vars.find(tokens[pos].get_value())->second;
    pos += 2;
    for (; pos < tokens.size(); ++pos) {
        if (tokens[pos].token_type == TokenType::close_bracket_token)
            break;
        params.push_back(parse_expression(tokens, pos));
    }
    CallFunction* res = new CallFunction(root, params);
    return std::shared_ptr<ASTNode>(res);
}

void AST::build_expression_ast(std::stack<std::shared_ptr<ASTNode>>& out_stack,
                               std::shared_ptr<ASTNode> curr) {
    if (out_stack.empty())
        return;

    // right operand
    if (std::dynamic_pointer_cast<Operator>(out_stack.top())) {
        auto next = out_stack.top();
        curr->add_child(next);
        out_stack.pop();
        build_expression_ast(out_stack, next);

    } else {
        curr->add_child(out_stack.top());
        out_stack.pop();
    }

    // left operand
    if (std::dynamic_pointer_cast<Operator>(out_stack.top())) {
        auto next = out_stack.top();
        curr->add_child(next);
        out_stack.pop();
        build_expression_ast(out_stack, next);

    } else {
        curr->add_child(out_stack.top());
        out_stack.pop();
    }
}


std::shared_ptr<ASTNode> AST::node_from_token(const Token& token) {
    std::shared_ptr<ASTNode> res;
    switch (token.token_type) {
        case TokenType::name_token: {
            Variable* v = new Variable(token.get_value());
            return  std::shared_ptr<ASTNode>(v);
        }
        case TokenType::literal_token: {
            Value* v = new Value(token.get_value());
            return std::shared_ptr<ASTNode>(v);
        }
        case TokenType::operator_token: {
            Operator* o = new Operator(token.get_value());
            return std::shared_ptr<ASTNode>(o);
        }
        default: {
            Value* v = new Value(token.get_value());
            return std::shared_ptr<ASTNode>(v);
        }
    }
}
