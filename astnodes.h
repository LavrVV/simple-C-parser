#ifndef ASTNODES_H
#define ASTNODES_H

#include <map>
#include <stack>
#include <memory>

#include "tokenizator.h"


enum ASTValType{
    String,
    Int,
    Float,
    Char,
    Bool,
    Void
};


enum OperatorType{
    Plus,
    Minus,
    Divide,
    Mod,
    Multiply,
    And,
    Or,
    Not
};


class Value;


class ASTNode{
public:
    ASTNode();

    virtual Value execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context);

    virtual void add_child(std::shared_ptr<ASTNode> child);

    virtual ~ASTNode() = default;

    std::vector<std::shared_ptr<ASTNode>> nodes;
};


class Value: public ASTNode{
public:
    explicit Value(std::string val);

    Value execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context) override;

    std::string get_value() const;

    bool is_return;
private:
    ASTValType valtype;

    std::string value;
};


class Variable: public ASTNode{
public:
    explicit Variable(std::string name);

    Value execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context) override;

private:
    std::string name;
};


class VariableDeclaration: public ASTNode{
public:
    VariableDeclaration(std::string name, std::string valtype);

    Value execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context) override;

private:
    ASTValType valtype;

    std::string name;
};


class Assign: public ASTNode{
public:
    Assign(std::string name, std::shared_ptr<ASTNode> what);

    Value execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context) override;

private:
    std::string name;
};


class Operator: public ASTNode{
public:
    explicit Operator(std::string val);

    Value execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context) override;

private:
    //std::string value;

    OperatorType op_type;
};


class Return: public ASTNode{
public:
    explicit Return(std::shared_ptr<ASTNode> expr);

    Value execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context) override;
};


class If: public ASTNode{
public:
    If(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> block);

    If(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> block,
       std::shared_ptr<ASTNode> else_block);

    Value execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context) override;

private:
    std::string value;
};


class While: public ASTNode{
public:
    While(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> block);

    Value execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context) override;

private:
    std::string value;
};


class For: public ASTNode{
public:
    For(std::shared_ptr<ASTNode> declaration, std::shared_ptr<ASTNode> condition,
        std::shared_ptr<ASTNode> increment, std::shared_ptr<ASTNode> block);

    Value execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context) override;

private:
    std::string value;
};


class Block: public ASTNode{
public:
    Block();

    Value execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context) override;
};


class Function: public ASTNode{
public:
    Function(std::string name, std::string return_type,
             std::vector<std::string> params, std::shared_ptr<ASTNode> root);

    Value execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context) override;

    const std::vector<std::string>& get_param_names() const;

private:
    std::string name;

    std::vector<std::string> params;

    ASTValType return_type;
};


class CallFunction: public ASTNode{
public:
    CallFunction(std::shared_ptr<ASTNode> root,
                 std::vector<std::shared_ptr<ASTNode>> params);

    Value execute(std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>& context) override;

    const std::vector<std::string>& get_param_names() const;

    //Function f;
private:
    std::shared_ptr<ASTNode> root;

    std::map<std::string, std::shared_ptr<ASTNode>> context;
};

#endif // ASTNODES_H
