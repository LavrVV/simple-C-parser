#include <stdexcept>
#include <libparser/base_astnode.hpp>


ASTNode::ASTNode() {

}


Value ASTNode::execute(Context& context) {
    return Value("null");
}


void ASTNode::add_child(std::shared_ptr<ASTNode> child) {
    this->nodes.push_back(child);
}

Value::Value(std::string val) {
    if (val[0] == '\'' and val.size() == 3) {
        this->valtype = ASTValType::Char;
        this->value = val.substr(1, 1);
    } else if (val[0] == '\"') {
        this->valtype = ASTValType::String;
        this->value = val.substr(1, val.size() - 1);
    } else if(std::isdigit(val[0]) or val[0] == '-') {
        try {
            std::stof(val);
            this->valtype = ASTValType::Float;
            this->value = val;
            return;
        } catch (std::invalid_argument e) {
            // TODO
        }
        try {
            std::stoi(val);
            this->valtype = ASTValType::Int;
            this->value = val;
            return;
        } catch (std::invalid_argument e) {
            // TODO
        }
    } else if (val == "true" or val == "false") {
        this->valtype = ASTValType::Bool;
        this->value = val;
    } else if (val == "" or val == "null") {
        this->valtype = ASTValType::Void;
        this->value = val;
    }
}


Value Value::execute(Context& context) {
    return *this;
}


std::string Value::get_value() const {
    return this->value;
}

ASTValType Value::get_valtype() const {
    return this->valtype;
}
