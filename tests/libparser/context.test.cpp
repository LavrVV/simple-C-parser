#include <catch2/catch_test_macros.hpp>
#include <libparser/context.hpp>
#include <libparser/base_astnode.hpp>

TEST_CASE("Test context with one scope", "[context]") {
    Context empty_context{};
    empty_context.push_scope();
    SECTION("get undefined var") {
        std::string var_name("a");
        auto var_value = empty_context.get_var_value(var_name);
        REQUIRE(var_value.get() == nullptr);
    }

    SECTION("set var") {
        std::string var_name("a");
        std::shared_ptr<ASTNode> var_value(new Value("value"));
        empty_context.add_var(var_name);
        empty_context.set_var_value(var_name, var_value);
        auto var_value_context = empty_context.get_var_value(var_name);
        REQUIRE(var_value_context == var_value);
    }
}

TEST_CASE("Test context with two scopes", "[context]") {
    Context empty_context{};
    empty_context.push_scope();
    empty_context.push_scope();
    SECTION("get undefined var") {
        std::string var_name("a");
        auto var_value = empty_context.get_var_value(var_name);
        REQUIRE(var_value.get() == nullptr);
    }

    SECTION("set var") {
        std::string var_name("a");
        std::shared_ptr<ASTNode> var_value(new Value("value"));
        empty_context.add_var(var_name);
        empty_context.set_var_value(var_name, var_value);
        auto var_value_context = empty_context.get_var_value(var_name);
        REQUIRE(var_value_context == var_value);
    }
    
    SECTION("pop scope") {
        std::string var_name("a");
        std::shared_ptr<ASTNode> var_value(new Value("value"));
        empty_context.add_var(var_name);
        empty_context.set_var_value(var_name, var_value);
        empty_context.pop_scope();
        auto var_value_context = empty_context.get_var_value(var_name);
        REQUIRE(var_value_context.get() == nullptr);
    }
}
