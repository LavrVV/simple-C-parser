#ifndef BASE_ASTNODE_H
#define BASE_ASTNODE_H

#include <libparser/context.hpp>

enum ASTValType {
    Int,
    Float,
    String,
    Char,
    Bool,
    Void
};


enum OperatorType {
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


class ASTNode {
public:
    ASTNode();

    /**
     * @brief vector of map variable_name -> variable_value
     */
    //using Context = std::vector<std::map<std::string, std::shared_ptr<ASTNode>>*>;

    virtual Value execute(Context& context);

    virtual void add_child(std::shared_ptr<ASTNode> child);

    virtual ~ASTNode() = default;

    std::vector<std::shared_ptr<ASTNode>> nodes;
};

/** dummy */
class Value: public ASTNode {
public:
    explicit Value(std::string val);

    Value execute(Context& context) override;

    std::string get_value() const;

    ASTValType get_valtype() const;

    bool is_return;
private:
    ASTValType valtype;

    std::string value;
};

#endif