#ifndef _KALEIDOSCOPE_TRAVERSING_VISITOR_HPP_
#define _KALEIDOSCOPE_TRAVERSING_VISITOR_HPP_

#include "AST.hpp"

namespace kaleidoscope {

class TraversingVisitor : public Visitor {
    void visit(const NumberExprAST&) {}

    void visit(const VariableExprAST&) {}

    void visit(const PrototypeAST&) {}

#define TRAVERSE(CLASS, CODE)                   \
    virtual void enter(const CLASS &) {}    \
    virtual void exit(const CLASS &) {}     \
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
}  // namespace kaleidoscope
#endif  // _KALEIDOSCOPE_TRAVERSING_VISITOR_HPP_

