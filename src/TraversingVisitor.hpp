#ifndef _KALEIDISCOPE_TRAVERSING_VISITOR_HPP_
#define _KALEIDISCOPE_TRAVERSING_VISITOR_HPP_

#include "AST.hpp"

namespace kaleidiscope {

class TraversingVisitor : public Visitor {
    void visit(const NumberExprAST&) {}

    void visit(const VariableExprAST&) {}

    void visit(const PrototypeAST&) {}

#define TRAVERSE(CLASS, CODE)                   \
    virtual void enter(const CLASS &node) {}    \
    virtual void exit(const CLASS &node) {}     \
    void visit(const CLASS &node) {             \
        enter(node);                            \
        CODE                                    \
        exit(node);                             \
    }                                           \

TRAVERSE(BinaryExprAST, {
    node.LHS->accept(this);
    node.RHS->accept(this);
})

TRAVERSE(CallExprAST, {
    for (auto& arg : node.Args) {
       arg->accept(this);
    }
})

TRAVERSE(FunctionAST, {
    node.Proto->accept(this);
    node.Body->accept(this);
})
};
}  // namespace kaleidiscope
#endif  // _KALEIDISCOPE_TRAVERSING_VISITOR_HPP_

