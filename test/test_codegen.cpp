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
#include <vector>
#include <memory>
#include <catch2/catch.hpp>

#include "CodegenVisitor.hpp"


using namespace kaleidoscope;

TEST_CASE("Generate Proto IR") {
    llvm::LLVMContext context;
    llvm::Module module("test", context);
    CodeGenVisitor generator(&module);
    PrototypeAST proto("f", std::vector<std::string>{"a", "b"});
    proto.accept(&generator);
    auto *protoIR = generator.getFunction();
    REQUIRE(protoIR != nullptr);
    REQUIRE(protoIR->arg_size() == 2);
    REQUIRE(protoIR->getReturnType()->isDoubleTy());
    REQUIRE(protoIR->getValueName()->first() == "f");
    REQUIRE(protoIR->getBasicBlockList().size() == 0);
}

TEST_CASE("Generate Function IR") {
    llvm::LLVMContext context;
    llvm::Module module("test", context);
    CodeGenVisitor generator(&module);
    FunctionAST fn(
            std::make_unique<PrototypeAST>("f", std::vector<std::string>{"a", "b"}),
            std::make_unique<BinaryExprAST>(
                '+',
                std::make_unique<VariableExprAST>("a"),
                std::make_unique<NumberExprAST>(1.0)));

    fn.accept(&generator);
    auto *fnIR = generator.getFunction();
    REQUIRE(fnIR != nullptr);
    REQUIRE(fnIR->arg_size() == 2);
    REQUIRE(fnIR->getReturnType()->isDoubleTy());
    REQUIRE(fnIR->getValueName()->first() == "f");
    REQUIRE(fnIR->getBasicBlockList().size() > 0);
    REQUIRE(fnIR->getEntryBlock().size() == 2);
}
