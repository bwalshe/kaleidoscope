#include <catch2/catch.hpp>
#include <memory>
#include <vector>
#include <string>

#include "AST.hpp"

using namespace kaleidiscope;
using NodePtr = std::unique_ptr<ASTNode>;
using ExprPtr = std::unique_ptr<ExprAST>;

TEST_CASE("NumberExprAST equality") {
    ExprPtr a = std::make_unique<NumberExprAST>(1.0);
    NodePtr b = std::make_unique<NumberExprAST>(1.0);
    NodePtr c = std::make_unique<VariableExprAST>("a");
    REQUIRE(*a == *b);
    REQUIRE(!(*a != *b));
    REQUIRE(*a != *c);
}

TEST_CASE("VariableASTNode equality") {
    NodePtr a = std::make_unique<VariableExprAST>("a");
    NodePtr a_ = std::make_unique<VariableExprAST>("a");
    NodePtr b = std::make_unique<VariableExprAST>("b");
    NodePtr c = std::make_unique<NumberExprAST>(1.0);
    REQUIRE(*a == *a_);
    REQUIRE(*a != *b);
    REQUIRE(*a != *c);
}

TEST_CASE("BinaryASTNode equality") {
    NodePtr a = std::make_unique<BinaryExprAST>('+',
            std::make_unique<NumberExprAST>(1.0),
            std::make_unique<NumberExprAST>(2.0));

    NodePtr b  = std::make_unique<BinaryExprAST>('+',
            std::make_unique<NumberExprAST>(1.0),
            std::make_unique<NumberExprAST>(2.0));

    NodePtr c = std::make_unique<BinaryExprAST>('+',
            std::make_unique<NumberExprAST>(2.0),
            std::make_unique<NumberExprAST>(1.0));
    NodePtr d  = std::make_unique<BinaryExprAST>('-',
            std::make_unique<NumberExprAST>(1.0),
            std::make_unique<NumberExprAST>(2.0));

    REQUIRE(*a == *b);
    REQUIRE(*a != *c);
    REQUIRE(*a != *d);
}

TEST_CASE("CallExprAST equality") {
    std::vector<ExprPtr> args1;
    args1.push_back(std::make_unique<NumberExprAST>(1.0));
    args1.push_back(std::make_unique<NumberExprAST>(2.0));

    std::vector<ExprPtr> args2;
    args2.push_back(std::make_unique<NumberExprAST>(1.0));
    args2.push_back(std::make_unique<NumberExprAST>(2.0));

    std::vector<ExprPtr> args3;
    args3.push_back(std::make_unique<NumberExprAST>(2.0));
    args3.push_back(std::make_unique<NumberExprAST>(1.0));

    NodePtr a = std::make_unique<CallExprAST>("f", std::move(args1));
    NodePtr b = std::make_unique<CallExprAST>("f", std::move(args2));
    NodePtr c = std::make_unique<CallExprAST>("f", std::move(args3));
    NodePtr d = std::make_unique<CallExprAST>("f", std::vector<ExprPtr>());

    REQUIRE(*a == *b);
    REQUIRE(*a != *c);
    REQUIRE(*a != *d);
}

TEST_CASE("PrototypeAST equality") {
    NodePtr a = std::make_unique<PrototypeAST>("f", std::vector<std::string>{"a", "b"});
    NodePtr b = std::make_unique<PrototypeAST>("f", std::vector<std::string>{"a", "b"});
    NodePtr c = std::make_unique<PrototypeAST>("f", std::vector<std::string>{"b", "c"});
    NodePtr d = std::make_unique<PrototypeAST>("f", std::vector<std::string>());

    REQUIRE(*a == *b);
    REQUIRE(*a != *c);
    REQUIRE(*a != *d);
}

TEST_CASE("FunctionAST equality") {
    NodePtr a = std::make_unique<FunctionAST>(
            std::make_unique<PrototypeAST>("f", std::vector<std::string>{"a", "b"}),
            std::make_unique<NumberExprAST>(1.0));

    NodePtr b = std::make_unique<FunctionAST>(
            std::make_unique<PrototypeAST>("f", std::vector<std::string>{"a", "b"}),
            std::make_unique<NumberExprAST>(1.0));

    NodePtr c = std::make_unique<FunctionAST>(
            std::make_unique<PrototypeAST>("g", std::vector<std::string>{"a", "b"}),
            std::make_unique<NumberExprAST>(1.0));

    REQUIRE(*a == *b);
    REQUIRE(*a != *c);
}


