#include <libparser/astnodes.hpp>

#include <cctype>
#include <libparser/context.hpp>

inline ASTValType read_type(const std::string& return_type) {
    if (return_type == "bool") {
        return ASTValType::Bool;
    } else if (return_type == "void") {
        return ASTValType::Void;
    } else if (return_type == "string") {
        return ASTValType::String;
    } else if (return_type == "int") {
        return  ASTValType::Int;
    } else if (return_type == "float") {
        return  ASTValType::Float;
    } else if (return_type == "char") {
        return  ASTValType::Char;
    }
    return ASTValType::Void;
}


Variable::Variable(std::string& name): name(name) {

}


Value Variable::execute(Context& context) {
    auto res = context.get_var_value(this->name);
    if (typeid(*res) == typeid(Value)) 
        return *std::dynamic_pointer_cast<Value>(res);
}


VariableDeclaration::VariableDeclaration(std::string& name, std::string& valtype): name(name) {
    this->valtype = read_type(valtype);
}


Value VariableDeclaration::execute(Context& context) {
    context.add_var(name);
    return Value("");
}


Assign::Assign(std::string& name, std::shared_ptr<ASTNode> what): name(name) {
    this->nodes.push_back(what);
}


Value Assign::execute(Context& context) {
    auto res = nodes[0]->execute(context);
    context.set_var_value(name, std::shared_ptr<ASTNode>(new Value(res)));
}


Operator::Operator(std::string& val) {
    if (val == "*") {
        this->op_type = OperatorType::Multiply;
    } else if (val == "/") {
        this->op_type = OperatorType::Divide;
    } else if (val == "+") {
        this->op_type = OperatorType::Plus;
    } else if (val == "-") {
        this->op_type = OperatorType::Minus;
    } else if (val == "%") {
        this->op_type = OperatorType::Mod;
    } else if (val == "!") {
        this->op_type = OperatorType::Not;
    } else if (val == "||") {
        this->op_type = OperatorType::Or;
    } else if (val == "&&") {
        this->op_type = OperatorType::And;
    } else {

    }
}

inline bool to_boolean(std::string& value) {
    if (value == "true")
        return true;
    else if (value == "false")
        return false;
    else
        return true; // default change if u want
}

inline std::string to_string(bool value) {
    return value ? "true" : "false";
}

Value Operator::execute(Context& context) {
    if (op_type == OperatorType::Not)
        return Value(to_string(!to_boolean(nodes[0]->execute(context).get_value())));
    auto left = nodes[1]->execute(context); 
    auto right = nodes[0]->execute(context);
    switch (op_type) {
        case OperatorType::Multiply:
            if (left.get_valtype() < 2 and right.get_valtype() < 2) {
                if (left.get_valtype() == ASTValType::Float or 
                    right.get_valtype() == ASTValType::Float) {
                    return Value(std::to_string(std::stof(left.get_value()) * std::stof(right.get_value())));
                } else {
                    return Value(std::to_string(std::stoi(left.get_value()) * std::stoi(right.get_value())));
                }
            }
            break;
        case OperatorType::Divide:
            if (left.get_valtype() < 2 and right.get_valtype() < 2) {
                if (left.get_valtype() == ASTValType::Float or 
                    right.get_valtype() == ASTValType::Float) {
                    return Value(std::to_string(std::stof(left.get_value()) / std::stof(right.get_value())));
                } else {
                    return Value(std::to_string(std::stoi(left.get_value()) / std::stoi(right.get_value())));
                }
            }
            break;
        case OperatorType::Plus:
            if (left.get_valtype() < 2 and right.get_valtype() < 2) {
                if (left.get_valtype() == ASTValType::Float or 
                    right.get_valtype() == ASTValType::Float) {
                    return Value(std::to_string(std::stof(left.get_value()) + std::stof(right.get_value())));
                } else {
                    return Value(std::to_string(std::stoi(left.get_value()) + std::stoi(right.get_value())));
                }
            } else if (left.get_valtype() == ASTValType::String or 
                       right.get_valtype() == ASTValType::String) {
                return Value(left.get_value() + right.get_value());
            }
            break;
        case OperatorType::Minus:
            if (left.get_valtype() < 2 and right.get_valtype() < 2) {
                if (left.get_valtype() == ASTValType::Float or 
                    right.get_valtype() == ASTValType::Float) {
                    return Value(std::to_string(std::stof(left.get_value()) - std::stof(right.get_value())));
                } else {
                    return Value(std::to_string(std::stoi(left.get_value()) - std::stoi(right.get_value())));
                }
            }
            break;
        case OperatorType::Mod:
            if (left.get_valtype() < 2 and right.get_valtype() < 2) {
                return Value(std::to_string(std::stoi(left.get_value()) % std::stoi(right.get_value())));
            }
            break;
        case OperatorType::Or:
            if (left.get_valtype() == ASTValType::Bool and 
                right.get_valtype() == ASTValType::Bool) {
                auto res = to_boolean(left.get_value()) and to_boolean(right.get_value());
                return Value(to_string(res)); 
            }
            break;
        case OperatorType::And:
            if (left.get_valtype() == ASTValType::Bool and 
                right.get_valtype() == ASTValType::Bool) {
                auto res = to_boolean(left.get_value()) or to_boolean(right.get_value());
                return Value(to_string(res)); 
            }
            break;
        default:
            return Value("");
            break;
    }
}


Return::Return(std::shared_ptr<ASTNode> expr) {
    this->nodes.push_back(expr);
}


Value Return::execute(Context& context) {
    auto res = nodes[0]->execute(context);
    res.is_return = true;
    return res;
}


If::If(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> block) {
    this->nodes.push_back(condition);
    this->nodes.push_back(block);
}


If::If(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> block,
       std::shared_ptr<ASTNode> else_block) {
    this->nodes.push_back(condition);
    this->nodes.push_back(block);
    this->nodes.push_back(else_block);
}


Value If::execute(Context& context) {
    context.push_scope();
    if (nodes[0]->execute(context).get_value() == "true") {
        return nodes[1]->execute(context);
    } else if (nodes.size() > 2) {
        return nodes[1]->execute(context);
    }
}


While::While(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> block) {
    this->nodes.push_back(condition);
    this->nodes.push_back(block);
}


Value While::execute(Context& context) {
    while (nodes[0]->execute(context).get_value() == "true") {
        nodes[1]->execute(context);
    }
    return Value("");
}


For::For(std::shared_ptr<ASTNode> declaration,
         std::shared_ptr<ASTNode> definition,
         std::shared_ptr<ASTNode> condition,
         std::shared_ptr<ASTNode> increment, 
         std::shared_ptr<ASTNode> block) {
    this->nodes.push_back(declaration);
    this->nodes.push_back(definition);
    this->nodes.push_back(condition);
    this->nodes.push_back(increment);
    this->nodes.push_back(block);
}


Value For::execute(Context& context) {
    context.push_scope();
    nodes[0]->execute(context);
    nodes[1]->execute(context);
    while (nodes[2]->execute(context).get_value() == "true") {
        nodes[3]->execute(context);
        nodes[4]->execute(context);
    }
    return Value("");
}


Block::Block() {

}


Value Block::execute(Context& context) {
    context.push_scope();
    for (const auto& node: nodes) { 
        auto res = node->execute(context); 
        if (res.is_return) {
            context.pop_scope();
            return res;
        }
    }
    context.pop_scope();
    return Value("");
}


Function::Function(std::string& name, std::string& return_type,
                   std::vector<std::string>& params, std::shared_ptr<ASTNode> root):
    name(name), params(params) {
    this->return_type = read_type(return_type);
    this->nodes.push_back(root);
}


Value Function::execute(Context& context) {
    return nodes[0]->execute(context);
}


const std::vector<std::string>& Function::get_param_names() const {
    return this->params;
}


CallFunction::CallFunction(std::shared_ptr<ASTNode> root,
                           std::vector<std::shared_ptr<ASTNode>>& params):
                           root(root) {
    this->nodes = params;
    //params
}

Value CallFunction::execute(Context& context) {
    context.push_scope();
    auto func = std::dynamic_pointer_cast<Function>(root);
    auto param_name = func->get_param_names().begin();
    for (const auto& param: nodes) {
        context.set_var_value(*param_name, param);
        param_name++;
    }
    auto res = root->execute(context);
    context.pop_scope();
    return res;
}


const std::vector<std::string>& CallFunction::get_param_names() const {
    std::shared_ptr<Function> f = std::dynamic_pointer_cast<Function>(root);
    return f->get_param_names();
}
