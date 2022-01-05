#ifndef _KALEIDISCOPE_AST_HPP_
#define _KALEIDISCOPE_AST_HPP_

#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <iostream>


namespace kaleidiscope {


class Visitor;

/// ASTNode
struct ASTNode {
  virtual ~ASTNode() = default;
  virtual void accept(Visitor *visitor) const = 0;
  virtual bool isEqual(const ASTNode &) const = 0;
  bool operator==(const ASTNode &other) const {
      return this == &other || isEqual(other);
  }

  bool operator!=(const ASTNode &other) const {
      return !(*this == other);
  }
};


/// ExprAST - Base class for all expression nodes.
struct ExprAST : public ASTNode {};

/// NumberASTNode - Expression class for numeric literals like "1.0".
struct NumberExprAST : public ExprAST {
  const double Val;

  explicit NumberExprAST(double Val) : Val(Val) {}

  void accept(Visitor *) const override;

  bool isEqual(const ASTNode &) const override;
};

/// VariableASTNode - Expression class for referencing a variable, like "a".
struct VariableExprAST : public ExprAST {
  const std::string Name;

  explicit VariableExprAST(const std::string &Name) : Name(Name) {}

  void accept(Visitor *) const override;

  bool isEqual(const ASTNode &) const override;
};

/// BinaryASTNode - Expression class for a binary operator.
struct BinaryExprAST : public ExprAST {
  const char Op;
  const std::unique_ptr<ExprAST> LHS, RHS;

  BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
      : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

  void accept(Visitor *) const override;

  bool isEqual(const ASTNode &) const override;
};

/// CallASTNode - Expression class for function calls.
struct CallExprAST : public ExprAST {
  const std::string Callee;
  const std::vector<std::unique_ptr<ExprAST>> Args;

 public:
  CallExprAST(const std::string &Callee,
              std::vector<std::unique_ptr<ExprAST>> Args)
      : Callee(Callee), Args(std::move(Args)) {}

  void accept(Visitor *) const override;

  bool isEqual(const ASTNode &) const override;
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
struct PrototypeAST : public ASTNode {
  const std::string Name;
  const std::vector<std::string> Args;

  PrototypeAST(const std::string &Name, std::vector<std::string> Args)
      : Name(Name), Args(std::move(Args)) {}

  void accept(Visitor *) const override;

  bool isEqual(const ASTNode &) const override;
};

/// FunctionAST - This class represents a function definition itself.
struct FunctionAST : public ASTNode {
  const std::unique_ptr<PrototypeAST> Proto;
  const std::unique_ptr<ExprAST> Body;

  FunctionAST(std::unique_ptr<PrototypeAST> Proto,
              std::unique_ptr<ExprAST> Body)

      : Proto(std::move(Proto)), Body(std::move(Body)) {}

  void accept(Visitor *) const override;

  bool isEqual(const ASTNode &) const override;
};


class Visitor {
 public:
    virtual void visit(const NumberExprAST&) = 0;
    virtual void visit(const VariableExprAST&) = 0;
    virtual void visit(const BinaryExprAST&) = 0;
    virtual void visit(const CallExprAST&) = 0;
    virtual void visit(const PrototypeAST&) = 0;
    virtual void visit(const FunctionAST&) = 0;
};

}  // namespace kaleidiscope
#endif  // _KALEIDISCOPE_AST_HPP_

