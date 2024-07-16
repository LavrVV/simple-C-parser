#ifndef CONTEXT_H
#define CONTEXT_H

#include <map>
#include <stack>
#include <memory>
#include <vector>
#include <string>

class ASTNode;

class Context {
public:
    using Scope = std::map<std::string, std::shared_ptr<ASTNode>>;
   
    Context();
    
    const std::shared_ptr<ASTNode> get_var_value(const std::string& name) const;
    
    void set_var_value(const std::string& name, std::shared_ptr<ASTNode> value);
    
    void add_var(const std::string& name);
    
    void push_scope(Scope* scope);
    
    void push_scope();
    
    void pop_scope();
    
    ~Context();
private:
    std::vector<Scope*> context;
};

#endif
