#include <catch2/catch.hpp>
#include <memory>
#include <vector>
#include <string>
#include "AST.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "ASTPrinter.hpp"

using namespace kaleidoscope;
using ExprPtr = std::unique_ptr<ExprAST>;

#define UNIQUE(TYPE, ...)       \
    std::make_unique<TYPE>(__VA_ARGS__) \


template <typename F>
void runParseTest(const std::string &text, const ASTNode &expected, F parseFn) {                       \
    Parser parser(UNIQUE(MemoryLexer, text));
    auto result = (parser.*parseFn)();
    REQUIRE(result);
#ifdef KALEIDOSCOPE_TESTS_PRINT_TREES
    ASTPrinter printer(std::cout);
    std::cout << '"' << text << "\" parses to:\n";
    result->accept(&printer);
    std::cout << std::endl;
#endif
    REQUIRE(*result == expected);
}                                                   

TEST_CASE("Parse number expression") {
    runParseTest("1", NumberExprAST(1.0), &Parser::ParseExpression);
}

TEST_CASE("Parse identifier expression") {
    runParseTest("a", VariableExprAST("a"), &Parser::ParseExpression);
}

TEST_CASE("Parse parenthesized expression") {
    BinaryExprAST expected('+', UNIQUE(NumberExprAST, 1), UNIQUE(NumberExprAST, 2));
    runParseTest("(1 + 2)", expected, &Parser::ParseExpression);
}

TEST_CASE("Parse call expression") {
    std::vector<ExprPtr> args;
    args.push_back(UNIQUE(NumberExprAST, 1));
    args.push_back(UNIQUE(NumberExprAST, 2));
    CallExprAST expected("f", std::move(args));
    runParseTest("f(1, 2)", expected, &Parser::ParseExpression);
}

TEST_CASE("Parse extern") {
    runParseTest("extern f(a b)", PrototypeAST("f", std::vector<std::string>{"a", "b"}), &Parser::ParseExtern);
}


TEST_CASE("Parse anon function") {
    auto proto = UNIQUE(PrototypeAST, Parser::ANON_FUNCTION_NAME, std::vector<std::string>());
    auto body = UNIQUE(NumberExprAST, 1);

    runParseTest("1", FunctionAST(std::move(proto),std::move(body)), &Parser::ParseTopLevelExpr);
}

TEST_CASE("Parse function declaration") {
    auto proto = UNIQUE(PrototypeAST, "f", std::vector<std::string>{"a", "b"});
    auto body = UNIQUE(NumberExprAST, 1);

    runParseTest("def f(a b) 1", FunctionAST(std::move(proto),std::move(body)), &Parser::ParseDefinition);
}

