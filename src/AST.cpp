#include "AST.hpp"

namespace  kaleidiscope {
#define ACCEPT(CLASS)                               \
    void CLASS::accept(Visitor *visitor) const {    \
        visitor->visit(*this);                      \
    }                                               \

#define EQUALITY(CLASS, TEST)                                           \
    bool CLASS::isEqual(const ASTNode &otherNode) const {               \
        const CLASS *other = dynamic_cast<const CLASS*>(&otherNode);    \
        return other && TEST;                                           \
    }                                                                   \

bool comparePtrs(std::unique_ptr<ExprAST> const &a,
        std::unique_ptr<ExprAST> const &b) {
    return a == b || *a == *b;
}

ACCEPT(NumberExprAST)
EQUALITY(NumberExprAST, Val == other->Val)

ACCEPT(VariableExprAST)
EQUALITY(VariableExprAST, Name == other->Name)

ACCEPT(BinaryExprAST)
EQUALITY(BinaryExprAST, Op == other->Op
        && *RHS == *other->RHS
        && *LHS == *other->LHS)

ACCEPT(CallExprAST)
EQUALITY(CallExprAST, Callee == other->Callee
        && Args.size() == other->Args.size()
        && std::equal(Args.begin(), Args.end(), other->Args.begin(), comparePtrs))

ACCEPT(PrototypeAST)
EQUALITY(PrototypeAST, Name == other->Name && Args == other->Args)

ACCEPT(FunctionAST)
EQUALITY(FunctionAST, *Proto == *other->Proto && *Body == *other->Body)
}  // end namespace kaleidiscope

