#include "astnodes.h"

#include <cctype>


inline ASTValType read_type(const std::string& return_type)
{
    if(return_type == "bool"){
        return ASTValType::Bool;
    }
    else if (return_type == "void") {
        return ASTValType::Void;
    }
    else if (return_type == "string") {
        return ASTValType::String;
    }
    else if (return_type == "int") {
        return  ASTValType::Int;
    }
    else if (return_type == "float") {
        return  ASTValType::Float;
    }
    else if (return_type == "char") {
        return  ASTValType::Char;
    }
    return ASTValType::Void;
}


ASTNode::ASTNode()
{

}


Value ASTNode::execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context)
{
    return  Value("null");
}


void ASTNode::add_child(std::shared_ptr<ASTNode> child)
{
    this->nodes.push_back(child);
}


Value::Value(std::string val)
{
    if(val[0] == '\'' and val.size() == 3){
        this->valtype = ASTValType::Char;
        this->value = val.substr(1, 1);
    }
    else if(val[0] == '\"'){
        this->valtype = ASTValType::String;
        this->value = val.substr(1, val.size() - 1);
    }
    else if(std::isdigit(val[0]) or val[0] == '-') {
        try {
            std::stof(val);
            this->valtype = ASTValType::Float;
            this->value = val;
            return;
        } catch (std::invalid_argument e) {

        }

        try {
            std::stoi(val);
            this->valtype = ASTValType::Int;
            this->value = val;
            return;
        } catch (std::invalid_argument e) {

        }
    } else if(val == "true" or val == "false") {
        this->valtype = ASTValType::Bool;
        this->value = val;
    } else if(val == "" or val == "null") {
        this->valtype = ASTValType::Void;
        this->value = val;
    }
}


Value Value::execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context)
{
    return *this;
}


std::string Value::get_value() const
{
    return this->value;
}


Variable::Variable(std::string name): name(name)
{

}


Value Variable::execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context)
{

}


VariableDeclaration::VariableDeclaration(std::string name, std::string valtype): name(name)
{
    this->valtype = read_type(valtype);
}


Value VariableDeclaration::execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context)
{

}


Assign::Assign(std::string name, std::shared_ptr<ASTNode> what): name(name)
{
    this->nodes.push_back(what);
}


Value Assign::execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context)
{

}


Operator::Operator(std::string val)
{
    if(val == "*"){
        this->op_type = OperatorType::Multiply;
    }
    else if(val == "/"){
        this->op_type = OperatorType::Divide;
    }
    else if(val == "+"){
        this->op_type = OperatorType::Plus;
    }
    else if(val == "-"){
        this->op_type = OperatorType::Minus;
    }
    else if(val == "%"){
        this->op_type = OperatorType::Mod;
    }
    else if(val == "!"){
        this->op_type = OperatorType::Not;
    }
    else if(val == "||"){
        this->op_type = OperatorType::Or;
    }
    else if(val == "&&"){
        this->op_type = OperatorType::And;
    }
    else{

    }
}


Value Operator::execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context)
{

}


Return::Return(std::shared_ptr<ASTNode> expr)
{
    this->nodes.push_back(expr);
}


Value Return::execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context)
{

}


If::If(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> block)
{
    this->nodes.push_back(condition);
    this->nodes.push_back(block);
}


If::If(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> block,
       std::shared_ptr<ASTNode> else_block)
{
    this->nodes.push_back(condition);
    this->nodes.push_back(block);
    this->nodes.push_back(else_block);
}


Value If::execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context)
{

}


While::While(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> block)
{
    this->nodes.push_back(condition);
    this->nodes.push_back(block);
}


Value While::execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context)
{

}


For::For(std::shared_ptr<ASTNode> declaration, std::shared_ptr<ASTNode> condition,
         std::shared_ptr<ASTNode> increment, std::shared_ptr<ASTNode> block)
{
    this->nodes.push_back(declaration);
    this->nodes.push_back(condition);
    this->nodes.push_back(increment);
    this->nodes.push_back(block);
}


Value For::execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context)
{

}


Block::Block()
{

}


Value Block::execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context)
{

}


Function::Function(std::string name, std::string return_type,
                   std::vector<std::string> params, std::shared_ptr<ASTNode> root):
    name(name), params(params)
{
    this->return_type = read_type(return_type);
    this->nodes.push_back(root);
}


Value Function::execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context)
{

}


const std::vector<std::string>& Function::get_param_names() const
{
    return this->params;
}


CallFunction::CallFunction(std::shared_ptr<ASTNode> root,
                           std::vector<std::shared_ptr<ASTNode>> params):
                           root(root)
{
    this->nodes = params;
    //params
}

Value CallFunction::execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context)
{

}


const std::vector<std::string>& CallFunction::get_param_names() const
{
    Function* f = dynamic_cast<Function*>(root.get());
    return f->get_param_names();
}
