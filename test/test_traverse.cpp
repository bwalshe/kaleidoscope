#include <iostream>
#include <memory>
#include <vector>
#include <catch2/catch.hpp>

#include "AST.hpp"
#include "TraversingVisitor.hpp"

namespace {

using namespace kaleidiscope;

enum Event {
    ENTER_BINOP,
    EXIT_BINOP,
    VISIT_NUM,
    VISIT_VAR,
    ENTER_CALL,
    EXIT_CALL,
    VISIT_PROTO,
    ENTER_FN,
    EXIT_FN
};

struct TracingVisitor : public TraversingVisitor {
    std::vector<Event> Events;

    void visit(const NumberExprAST &) {
        Events.push_back(VISIT_NUM);
    }

    void visit(const VariableExprAST &) {
        Events.push_back(VISIT_VAR);
    }

    void enter(const BinaryExprAST &) {
        Events.push_back(ENTER_BINOP);
    }

    void exit(const BinaryExprAST &) {
        Events.push_back(EXIT_BINOP);
    }

    void enter(const CallExprAST &) {
        Events.push_back(ENTER_CALL);
    }

    void exit(const CallExprAST &) {
        Events.push_back(EXIT_CALL);
    }

    void visit(const PrototypeAST &) {
        Events.push_back(VISIT_PROTO);
    }

    void enter(const FunctionAST &) {
        Events.push_back(ENTER_FN);
    }

    void exit(const FunctionAST &) {
        Events.push_back(EXIT_FN);
    }
};


using NodePtr = std::unique_ptr<ASTNode>;
using ExprPtr = std::unique_ptr<ExprAST>;

ExprPtr num(double val) {
    return std::make_unique<NumberExprAST>(val);
}


ExprPtr var(const std::string &name) {
    return std::make_unique<VariableExprAST>(name);
}


ExprPtr binary(char op, ExprPtr &&l, ExprPtr &&r) {
    return std::make_unique<BinaryExprAST>(op, std::move(l), std::move(r));
}

ExprPtr call(const std::string &callee, ExprPtr &&argA, ExprPtr &&argB) {
    std::vector<ExprPtr> args;
    args.push_back(std::move(argA));
    args.push_back(std::move(argB));
    return std::make_unique<CallExprAST>(callee, std::move(args));
}

std::unique_ptr<PrototypeAST> proto(const std::string &name) {
    std::vector<std::string> args;
    return std::make_unique<PrototypeAST>(name, args);
}

std::unique_ptr<FunctionAST> function(const std::string &name, ExprPtr &&body) {
    return std::make_unique<FunctionAST>(proto(name), std::move(body));
}

}  // end anonymous namespace


#define TRAVERSAL_TEST(TYPE, AST, EVENTS)                   \
    TEST_CASE("Test " #TYPE " traversal") {                 \
        using Catch::Matchers::Equals;                      \
        TracingVisitor visitor;                             \
        auto node = AST;                                    \
        node->accept(&visitor);                             \
        REQUIRE_THAT(visitor.Events, Equals<Event>EVENTS);  \
    }                                                       \

TRAVERSAL_TEST(
    NumberExprAST,
    num(1.0),
    ({VISIT_NUM})
)

TRAVERSAL_TEST(
    VariableExprAST,
    var("a"),
    ({VISIT_VAR})
)

TRAVERSAL_TEST(
    BinExprAST,
    binary('+', num(1.0), var("a")),
    ({ENTER_BINOP, VISIT_NUM, VISIT_VAR, EXIT_BINOP})
)


TRAVERSAL_TEST(
    CallExprAST,
    call("fn", num(1.0), var("a")),
    ({ENTER_CALL, VISIT_NUM, VISIT_VAR, EXIT_CALL})
)


TRAVERSAL_TEST(
    PrototypeAST,
    proto("fn"),
    ({VISIT_PROTO})
)

TRAVERSAL_TEST(
    FunctionAST,
    function("fn", num(1.0)),
    ({ENTER_FN, VISIT_PROTO, VISIT_NUM, EXIT_FN})
)
