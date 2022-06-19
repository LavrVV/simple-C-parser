#include <libparser/context.hpp>

Context::Context(): context() {
}

std::shared_ptr<ASTNode> Context::get_var_value(std::string& name) {
    for (auto c = context.rbegin(); c < context.rend(); c++) {
        try {
            return (*c)->at(name);
        } catch (std::out_of_range exp) {}
    }
    return std::shared_ptr<ASTNode>();
}

void Context::add_var(std::string& name) {
    (*(*context.end()))[name] = std::shared_ptr<ASTNode>(nullptr);
}

void Context::set_var_value(std::string name, std::shared_ptr<ASTNode> value) {
    for (auto c = context.rbegin(); c < context.rend(); c++) {
        try {
            (*c)->at(name);
            (*(*c))[name] = value;
            return;
        } catch (std::out_of_range exp) {}
    }
}

void Context::push_scope(Scope* scope) {
    context.push_back(scope);
}

void Context::push_scope() {
    context.push_back(new Scope());
}

void Context::pop_scope() {
    delete *context.end();
    context.pop_back();
}

Context::~Context() {
    while (!context.empty()){
        pop_scope();
    }
    
}