#include <catch2/catch_test_macros.hpp>
#include <libparser/tokenizator.hpp>

TEST_CASE("Test read_operator", "[tokenizator]") {
    SECTION("single char operator") {
        std::vector<std::string> operators = {"*", "/", "+", "-", "%", "!", ",", ";"};
        for (auto op: operators) {
            auto tokens = tokenize(op);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0].token_type == TokenType::operator_token);
            REQUIRE(tokens[0].get_value() == op);    
        }
    }

    SECTION("two char operator") {
        std::vector<std::string> operators = {"&&", "||", "==", "!="};
        for (auto op: operators) {
            auto tokens = tokenize(op);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0].token_type == TokenType::operator_token);
            REQUIRE(tokens[0].get_value() == op);    
        }
    }
}

TEST_CASE("Test read_name", "[tokenizator]") {
    SECTION("some name") {
        std::vector<std::string> names = {"aaaa", "AAA", "safk_lh54l"};
        for (auto name: names) {
            auto tokens = tokenize(name);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0].token_type == TokenType::name_token);
            REQUIRE(tokens[0].get_value() == name);    
        }
    }

    SECTION("literal keyword") {
        std::vector<std::string> literals = {"true", "false"};
        for (auto literal: literals) {
            auto tokens = tokenize(literal);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0].token_type == TokenType::literal_token);
            REQUIRE(tokens[0].get_value() == literal);    
        }
    }

    SECTION("operator keyword") {
        std::vector<std::string> operators = {"if", "else", "while", "for", "return"};
        for (auto op: operators) {
            auto tokens = tokenize(op);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0].token_type == TokenType::operator_token);
            REQUIRE(tokens[0].get_value() == op);    
        }
    }

    SECTION("type keyword") {
        std::vector<std::string> types = {"int", "float", "char", "bool", "string", "void"};
        for (auto t: types) {
            auto tokens = tokenize(t);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0].token_type == TokenType::type_token);
            REQUIRE(tokens[0].get_value() == t);    
        }
    }
}

TEST_CASE("Test read_literal", "[tokenizator]") {
    SECTION("char") {
        std::vector<std::string> literals = {"\'g\'"};
        for (auto literal: literals) {
            auto tokens = tokenize(literal);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0].token_type == TokenType::literal_token);
            REQUIRE(tokens[0].get_value() == literal);    
        }
    }

    SECTION("string") {
        std::vector<std::string> literals = {"\"true\"", "\"false\""};
        for (auto literal: literals) {
            auto tokens = tokenize(literal);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0].token_type == TokenType::literal_token);
            REQUIRE(tokens[0].get_value() == literal);    
        }
    }

    SECTION("int") {
        std::vector<std::string> literals = {"143", "644313"};
        for (auto literal: literals) {
            auto tokens = tokenize(literal);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0].token_type == TokenType::literal_token);
            REQUIRE(tokens[0].get_value() == literal);    
        }
    }

    SECTION("float") {
        std::vector<std::string> literals = {"41231.654", "1257.14"};
        for (auto literal: literals) {
            auto tokens = tokenize(literal);
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0].token_type == TokenType::literal_token);
            REQUIRE(tokens[0].get_value() == literal);    
        }
    }
}

TEST_CASE("Test more than 1 tokens", "[tokenizator]") {
    SECTION("empty string") {
        std::string code("");
        auto tokens = tokenize(code);
        REQUIRE(tokens.size() == 0);
    }

    SECTION("empty main") {
        std::string code("int main() {}");
        auto tokens = tokenize(code);
        REQUIRE(tokens.size() == 6);
        REQUIRE(tokens[0].get_value() == "int");
        REQUIRE(tokens[0].token_type == TokenType::type_token);
        REQUIRE(tokens[1].get_value() == "main");
        REQUIRE(tokens[1].token_type == TokenType::name_token);
        REQUIRE(tokens[2].get_value() == "(");
        REQUIRE(tokens[2].token_type == TokenType::open_bracket_token);
        REQUIRE(tokens[3].get_value() == ")");
        REQUIRE(tokens[3].token_type == TokenType::close_bracket_token);
        REQUIRE(tokens[4].get_value() == "{");
        REQUIRE(tokens[4].token_type == TokenType::open_bracket_token);
        REQUIRE(tokens[5].get_value() == "}");
        REQUIRE(tokens[5].token_type == TokenType::close_bracket_token);
    }

    SECTION("some expression") {
        std::string code("int x = 12+1.1/15 || true;");
        auto tokens = tokenize(code);
        REQUIRE(tokens.size() == 11);
        REQUIRE(tokens[0].get_value() == "int");
        REQUIRE(tokens[0].token_type == TokenType::type_token);
        REQUIRE(tokens[1].get_value() == "x");
        REQUIRE(tokens[1].token_type == TokenType::name_token);
        REQUIRE(tokens[2].get_value() == "=");
        REQUIRE(tokens[2].token_type == TokenType::operator_token);
        REQUIRE(tokens[3].get_value() == "12");
        REQUIRE(tokens[3].token_type == TokenType::literal_token);
        REQUIRE(tokens[4].get_value() == "+");
        REQUIRE(tokens[4].token_type == TokenType::operator_token);
        REQUIRE(tokens[5].get_value() == "1.1");
        REQUIRE(tokens[5].token_type == TokenType::literal_token);
        REQUIRE(tokens[6].get_value() == "/");
        REQUIRE(tokens[6].token_type == TokenType::operator_token);
        REQUIRE(tokens[7].get_value() == "15");
        REQUIRE(tokens[7].token_type == TokenType::literal_token);
        REQUIRE(tokens[8].get_value() == "||");
        REQUIRE(tokens[8].token_type == TokenType::operator_token);
        REQUIRE(tokens[9].get_value() == "true");
        REQUIRE(tokens[9].token_type == TokenType::literal_token);
        REQUIRE(tokens[10].get_value() == ";");
        REQUIRE(tokens[10].token_type == TokenType::operator_token);
    }
}
