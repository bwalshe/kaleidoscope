#ifndef _KALEIDISCOPE_AST_PRINTER_HPP_
#define _KALEIDISCOPE_AST_PRINTER_HPP_

#include <iostream>
#include <catch2/catch.hpp>

#include "AST.hpp"
#include "TraversingVisitor.hpp"


namespace kaleidiscope {


class ASTPrinter : public TraversingVisitor {
    int IndentLevel;
    std::ostream &Out;

    void visit(const NumberExprAST &expr) {
        indent() << "Number: " << expr.Val << '\n';
    }

    void visit(const VariableExprAST &expr) {
        indent() << "Id: " << expr.Name << '\n';
    }

    void enter(const BinaryExprAST &expr) {
        indent() << "Op: " << expr.Op << '\n';
        ++IndentLevel;
    }

    void exit(const kaleidiscope::BinaryExprAST &) {
        --IndentLevel;
    }

    void enter(const CallExprAST &expr) {
        indent() << "Call: " << expr.Callee << '\n';
        ++IndentLevel;
    }

    void exit(const CallExprAST &) {
        --IndentLevel;
    }

    void visit(const PrototypeAST &proto) {
        auto& out = indent();
        out << "Proto " <<proto.Name << "(";
        for (auto& arg : proto.Args) {
            out << arg << " ";
        }
        out << ")\n";
    }

    void enter(const FunctionAST &) {
        indent() << "Function: \n";
        ++IndentLevel;
    }

    void exit(const FunctionAST &) {
        --IndentLevel;
    }

    std::ostream &indent() const {
        for (int i = 0; i < IndentLevel; ++i) {
            Out << "  ";
        }
        return Out;
    }

 public:
    explicit ASTPrinter(std::ostream &Out) : IndentLevel(0), Out(Out)  {}
};

}  // end namespace kaleidiscope

#endif  //  _KALEIDISCOPE_AST_PRINTER_HPP_

