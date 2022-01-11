#ifndef _KALEIDOSCOPE_CODEGEN_VISITOR_HPP_
#define _KALEIDOSCOPE_CODEGEN_VISITOR_HPP_

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <memory>
#include <string>
#include <stack>
#include <vector>
#include <map>

#include "TraversingVisitor.hpp"
#include "AST.hpp"


namespace kaleidoscope {



class CodeGenVisitor : public TraversingVisitor {
    llvm::LLVMContext &TheContext;
    llvm::Module *TheModule;
    std::unique_ptr<llvm::IRBuilder<>> Builder;
    std::map<std::string, llvm::Value*> NamedValues;
    std::stack<llvm::Value *> ValueStack;
    llvm::Function *OutputFunction;

 public:
    explicit CodeGenVisitor(llvm::Module *module) :
        TheContext(module->getContext()),
        TheModule(module),
        Builder(std::make_unique<llvm::IRBuilder<>>(TheContext)),
        OutputFunction(nullptr) {}

    void visit(const NumberExprAST& expr) override;

    void visit(const VariableExprAST &) override; 

    void exit(const BinaryExprAST &expr) override;

    void exit(const CallExprAST &expr) override;

    void visit(const PrototypeAST &proto) override;
    
    void visit(const FunctionAST &fn) override;

    inline llvm::Function const *getFunction() const {
        return OutputFunction;
    }
};
}  // end namespace kaleidoscope
#endif  // _KALEIDOSCOPE_CODEGEN_VISITOR_HPP_
