#ifndef ASTNODES_H
#define ASTNODES_H

#include <libparser/tokenizator.hpp>
#include <libparser/base_astnode.hpp>


class Variable: public ASTNode {
public:
    explicit Variable(std::string name);

    /** return varibale value by name */
    Value execute(Context& context) override;

private:
    std::string name;
};


class VariableDeclaration: public ASTNode {
public:
    VariableDeclaration(std::string name, std::string valtype);
    
    /** add var to current context*/
    Value execute(Context& context) override;

private:
    ASTValType valtype;

    std::string name;
};


class Assign: public ASTNode {
public:
    Assign(std::string name, std::shared_ptr<ASTNode> what);

    /** set variable value by name */
    Value execute(Context& context) override;

private:
    std::string name;
};


class Operator: public ASTNode {
public:
    explicit Operator(std::string val);

    Value execute(Context& context) override;

private:
    //std::string value;

    OperatorType op_type;
};


class Return: public ASTNode {
public:
    explicit Return(std::shared_ptr<ASTNode> expr);

    /** finish function and ret expr.exec */
    Value execute(Context& context) override;
};


class If: public ASTNode {
public:
    If(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> block);

    If(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> block,
       std::shared_ptr<ASTNode> else_block);
    
    Value execute(Context& context) override;

private:
    std::string value;
};


class While: public ASTNode {
public:
    While(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> block);

    Value execute(Context& context) override;

private:
    std::string value;
};


class For: public ASTNode {
public:
    For(std::shared_ptr<ASTNode> declaration, 
        std::shared_ptr<ASTNode> definiton,
        std::shared_ptr<ASTNode> condition,
        std::shared_ptr<ASTNode> increment,
        std::shared_ptr<ASTNode> block);

    Value execute(Context& context) override;

private:
    std::string value;
};


class Block: public ASTNode {
public:
    Block();

    Value execute(Context& context) override;
};


class Function: public ASTNode {
public:
    Function(std::string name, std::string return_type,
             std::vector<std::string> params, std::shared_ptr<ASTNode> root);

    Value execute(Context& context) override;

    const std::vector<std::string>& get_param_names() const;

private:
    std::string name;

    std::vector<std::string> params;

    ASTValType return_type;
};


class CallFunction: public ASTNode {
public:
    CallFunction(std::shared_ptr<ASTNode> root,
                 std::vector<std::shared_ptr<ASTNode>> params);

    Value execute(Context& context) override;

    const std::vector<std::string>& get_param_names() const;

    //Function f;
private:
    std::shared_ptr<ASTNode> root;

    std::map<std::string, std::shared_ptr<ASTNode>> context;
};

#endif // ASTNODES_H