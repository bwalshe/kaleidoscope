#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <memory>
#include <iostream>

#include "Parser.hpp"
#include "Lexer.hpp"
#include "CodegenVisitor.hpp"
#include "ASTPrinter.hpp"

int main(int argc, char **argv) {
    using namespace kaleidoscope;
    llvm::LLVMContext context;
    llvm::Module module("IRPrinter", context);
    CodeGenVisitor codegen(&module);

    Parser parser(std::make_unique<StdInLexer>());
    ASTPrinter printer(std::cout);
    std::cout << "Ready> "; 
    
    while (auto ast = parser.NextTopLevelASTNode()) {
        std::cout << "AST:\n";
        ast->accept(&printer);
        std::cout << '\n';

        ast->accept(&codegen);


        if(auto *f = codegen.getFunction()) {
            std::cout << "IR:\n";
            f->print(llvm::outs());
            if(CodeGenVisitor::isAnon(f)) {
                codegen.getFunction()->eraseFromParent();
            }
            std::cout << std::endl;
        }

        std::cout << "Ready> "; 
    }

    std::cout << "\n\n----------\n" << std::endl;
    module.print(llvm::outs(), nullptr);
}
