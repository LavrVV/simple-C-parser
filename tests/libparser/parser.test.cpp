#include <typeinfo>
#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <libparser/parser.hpp>

TEST_CASE("Test parser", "[parser]") {
    SECTION("parse simple main") {
        std::vector<Token> input{
            Token("int", TokenType::type_token),
            Token("main", TokenType::name_token),
            Token("(", TokenType::open_bracket_token),
            Token(")", TokenType::close_bracket_token),
            Token("{", TokenType::open_bracket_token),
            Token("}", TokenType::close_bracket_token)
        };
        AST ast(input);
        auto root = ast.get_root();
        
        REQUIRE(root.size() == 2);
        REQUIRE(typeid(*root[0].get()) == typeid(Function));
        REQUIRE(typeid(*root[1].get()) == typeid(CallFunction));
        REQUIRE(dynamic_cast<Function*>(root[0].get())->get_param_names().empty()); // no params
        REQUIRE(dynamic_cast<Function*>(root[0].get())->nodes.size() == 1); // only one block
        REQUIRE(typeid(*dynamic_cast<Function*>(root[0].get())->nodes[0].get()) == typeid(Block));
        REQUIRE(dynamic_cast<Function*>(root[0].get())->nodes[0]->nodes.empty()); // block is empty
    }
    
    SECTION("parse expression") {
        std::vector<Token> input{
            Token("int", TokenType::type_token),
            Token("func", TokenType::name_token),
            Token("(", TokenType::open_bracket_token),
            Token(")", TokenType::close_bracket_token),
            Token("{", TokenType::open_bracket_token),
            Token("}", TokenType::close_bracket_token),
            Token("int", TokenType::type_token),
            Token("x", TokenType::name_token),
            Token("=", TokenType::operator_token),
            Token("(", TokenType::open_bracket_token),
            Token("12", TokenType::literal_token),
            Token("+", TokenType::operator_token, 1),
            Token("func", TokenType::name_token),
            Token("(", TokenType::open_bracket_token),
            Token(")", TokenType::close_bracket_token),
            Token(")", TokenType::close_bracket_token),
            Token("*", TokenType::operator_token, 2),
            Token("y", TokenType::name_token),
            Token(";", TokenType::operator_token)
        };
        AST ast(input);
        auto root = ast.get_root();

        REQUIRE(root.size() == 4);
        REQUIRE(typeid(*root[0].get()) == typeid(Function));
        REQUIRE(typeid(*root[1].get()) == typeid(VariableDeclaration));
        REQUIRE(typeid(*root[2].get()) == typeid(Assign));
        REQUIRE(typeid(*root[3].get()) == typeid(CallFunction));

        auto assign_nodes = root[2]->nodes;
        REQUIRE(assign_nodes.size() == 1);
        REQUIRE(typeid(*assign_nodes[0].get()) == typeid(Operator));
        REQUIRE(assign_nodes[0]->nodes.size() == 2);
        std::cout << typeid(*assign_nodes[0]->nodes[0].get()).name() << std::endl;
        REQUIRE(typeid(*assign_nodes[0]->nodes[1].get()) == typeid(Operator));
        REQUIRE(typeid(*assign_nodes[0]->nodes[0].get()) == typeid(Variable));

        auto left_nodes = assign_nodes[0]->nodes[1]->nodes;
        REQUIRE(left_nodes.size() == 2);
        REQUIRE(typeid(*left_nodes[1].get()) == typeid(Value));
        REQUIRE(typeid(*left_nodes[0].get()) == typeid(CallFunction));
    }
    
    SECTION("parse if") {
        std::vector<Token> input{
            Token("if", TokenType::operator_token),
            Token("(", TokenType::open_bracket_token),
            Token("a", TokenType::name_token),
            Token("==", TokenType::operator_token),
            Token("b", TokenType::name_token),
            Token(")", TokenType::close_bracket_token),
            Token("{", TokenType::open_bracket_token),
            Token("}", TokenType::close_bracket_token),
            Token("else", TokenType::operator_token),
            Token("{", TokenType::open_bracket_token),
            Token("}", TokenType::close_bracket_token)
        };
        AST ast(input);
        auto root = ast.get_root();

        REQUIRE(root.size() == 2);
        REQUIRE(typeid(*root[0].get()) == typeid(If));
        REQUIRE(typeid(*root[1].get()) == typeid(CallFunction));

        auto if_stmt = dynamic_cast<If*>(root[0].get());
        REQUIRE(if_stmt->nodes.size() == 3);
        REQUIRE(typeid(*if_stmt->nodes[0].get()) == typeid(Operator));
        REQUIRE(if_stmt->nodes[1]->nodes.size() == 0);
        REQUIRE(if_stmt->nodes[2]->nodes.size() == 0);
    }

    SECTION("parse while") {
        std::vector<Token> input{
            Token("while", TokenType::operator_token),
            Token("(", TokenType::open_bracket_token),
            Token("x", TokenType::name_token),
            Token("==", TokenType::operator_token),
            Token("12", TokenType::literal_token),
            Token(")", TokenType::close_bracket_token),
            Token("{", TokenType::open_bracket_token),
            Token("}", TokenType::close_bracket_token)
        };
        AST ast(input);
        auto root = ast.get_root();

        REQUIRE(root.size() == 2);
        REQUIRE(typeid(*root[0].get()) == typeid(While));
        REQUIRE(typeid(*root[1].get()) == typeid(CallFunction));

        auto while_stmt = dynamic_cast<While*>(root[0].get());
        REQUIRE(while_stmt->nodes.size() == 2);
        REQUIRE(typeid(*while_stmt->nodes[0].get()) == typeid(Operator));
        REQUIRE(while_stmt->nodes[1]->nodes.size() == 0);
    }

    SECTION("parse for") {
        std::vector<Token> input{
            Token("for", TokenType::operator_token),
            Token("(", TokenType::open_bracket_token),
            Token("int", TokenType::type_token),
            Token("i", TokenType::name_token),
            Token("=", TokenType::operator_token),
            Token("0", TokenType::literal_token),
            Token(";", TokenType::operator_token),
            Token("i", TokenType::name_token),
            Token("!=", TokenType::operator_token),
            Token("10", TokenType::literal_token),
            Token(";", TokenType::operator_token),
            Token("i", TokenType::name_token),
            Token("=", TokenType::operator_token),
            Token("i", TokenType::name_token),
            Token("+", TokenType::operator_token),
            Token("1", TokenType::literal_token),
            Token(")", TokenType::close_bracket_token),
            Token("{", TokenType::open_bracket_token),
            Token("}", TokenType::close_bracket_token),
        };
        AST ast(input);
        auto root = ast.get_root();

        REQUIRE(root.size() == 2);
        REQUIRE(typeid(*root[0].get()) == typeid(For));
        REQUIRE(typeid(*root[1].get()) == typeid(CallFunction));

        auto for_stmt = dynamic_cast<For*>(root[0].get());
        REQUIRE(for_stmt->nodes.size() == 5);
        REQUIRE(typeid(*for_stmt->nodes[0].get()) == typeid(VariableDeclaration));
        REQUIRE(typeid(*for_stmt->nodes[1].get()) == typeid(Assign));
        REQUIRE(typeid(*for_stmt->nodes[2].get()) == typeid(Operator));
        REQUIRE(typeid(*for_stmt->nodes[3].get()) == typeid(Assign));
        REQUIRE(typeid(*for_stmt->nodes[4].get()) == typeid(Block));
        REQUIRE(for_stmt->nodes[4]->nodes.size() == 0);
    }
}
