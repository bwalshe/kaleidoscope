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


void runParseTest(const std::string &text, const ASTNode &expected) {                       \
    Parser parser(UNIQUE(MemoryLexer, text));
    auto result = parser.NextTopLevelASTNode();
    REQUIRE(result);
#ifdef KALEIDOSCOPE_TESTS_PRINT_TREES
    ASTPrinter printer(std::cout);
    std::cout << '"' << text << "\" parses to:\n";
    result->accept(&printer);
    std::cout << std::endl;
#endif
    REQUIRE(*result == expected);
}                                                   

FunctionAST anonFn(std::unique_ptr<ExprAST> &&exp) {
    auto prototype = std::make_unique<PrototypeAST>(Parser::ANON_FUNCTION_NAME,
            std::vector<std::string>());
    return FunctionAST(std::move(prototype), std::move(exp));
}
TEST_CASE("Parse number expression") {

    runParseTest("1", anonFn(std::make_unique<NumberExprAST>(1.0)));
}

TEST_CASE("Parse identifier expression") {
    runParseTest("a", anonFn(std::make_unique<VariableExprAST>("a")));
}

TEST_CASE("Parse parenthesized expression") {
    auto expected = std::make_unique<BinaryExprAST>('+',
            UNIQUE(NumberExprAST, 1),
            UNIQUE(NumberExprAST, 2));
    runParseTest("(1 + 2)", anonFn(std::move(expected)));
}

TEST_CASE("Parse call expression") {
    std::vector<ExprPtr> args;
    args.push_back(UNIQUE(NumberExprAST, 1));
    args.push_back(UNIQUE(NumberExprAST, 2));
    auto expected = std::make_unique<CallExprAST>("f", std::move(args));
    runParseTest("f(1, 2)", anonFn(std::move(expected)));
}

TEST_CASE("Parse extern") {
    runParseTest("extern f(a b)", PrototypeAST("f", std::vector<std::string>{"a", "b"}));
}

TEST_CASE("Parse function declaration") {
    auto proto = UNIQUE(PrototypeAST, "f", std::vector<std::string>{"a", "b"});
    auto body = UNIQUE(NumberExprAST, 1);

    runParseTest("def f(a b) 1", FunctionAST(std::move(proto),std::move(body)));
}

