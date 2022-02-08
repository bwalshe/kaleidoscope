#include "CodegenVisitor.hpp"

namespace kaleidoscope {

void LogError(const char *Str) {
    fprintf(stderr, "Error: %s\n", Str);
}


void CodeGenVisitor::visit(const NumberExprAST& expr) {
    ValueStack.push(llvm::ConstantFP::get(TheContext,
                llvm::APFloat(expr.Val)));
}


void CodeGenVisitor::visit(const VariableExprAST &expr) {
    auto V = NamedValues[expr.Name];
    if (!V)
        LogError("Unknown variable name");
    ValueStack.push(V);
}


void CodeGenVisitor::exit(const BinaryExprAST &expr) {
    auto *R = ValueStack.top();
    ValueStack.pop();
    auto *L = ValueStack.top();
    ValueStack.pop();

    switch (expr.Op) {
        case '+':
            ValueStack.push(Builder->CreateFAdd(L, R, "addtmp"));
            break;
        case '-':
            ValueStack.push(Builder->CreateFSub(L, R, "subtmp"));
            break;
        case '*':
            ValueStack.push(Builder->CreateFMul(L, R, "multmp"));
            break;
        case '<':
            L = Builder->CreateFCmpULT(L, R, "cmptmp");
            // Convert bool 0/1 to double 0.0 or 1.0
            ValueStack.push(Builder->CreateUIToFP(L,
                        llvm::Type::getDoubleTy(TheContext), "booltmp"));
            break;
        default:
            LogError("invalid binary operator");
    }
}


void CodeGenVisitor::exit(const CallExprAST &expr) {
    llvm::Function *CalleeF = TheModule->getFunction(expr.Callee);
    if (!CalleeF)  {
        LogError("Unknown function referenced");
        return;
    }

    // If argument mismatch error.
    if (CalleeF->arg_size() != expr.Args.size()) {
        LogError("Incorrect # arguments passed");
        return;
    }

    std::vector<llvm::Value *> ArgsV;

    for (unsigned i = 0, e = expr.Args.size(); i != e; ++i) {
        ArgsV.push_back(ValueStack.top());
        ValueStack.pop();
        if (!ArgsV.back())
            return;
    }

    ValueStack.push(Builder->CreateCall(CalleeF, ArgsV, "calltmp"));
}


void CodeGenVisitor::visit(const PrototypeAST &proto) {
    // Make the function type:  double(double,double) etc.
    std::vector<llvm::Type *> Doubles(proto.Args.size(),
            llvm::Type::getDoubleTy(TheContext));
    llvm::FunctionType *FT =
        llvm::FunctionType::get(llvm::Type::getDoubleTy(TheContext),
                Doubles, false);

    llvm::Function *F =
        llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                proto.Name, TheModule);

    // Set names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg : F->args())
        Arg.setName(proto.Args[Idx++]);

    OutputFunction = F;
}


void CodeGenVisitor::visit(const FunctionAST &fn) {
    llvm::Function *TheFunction = TheModule->getFunction(fn.Proto->Name);

    if (!TheFunction) {
        OutputFunction = nullptr;
        fn.Proto->accept(this);
        TheFunction = OutputFunction;
    }

    if (!TheFunction)
        return;

    // Create a new basic block to start insertion into.
    llvm::BasicBlock *BB =
        llvm::BasicBlock::Create(TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    NamedValues.clear();
    for (auto &Arg : TheFunction->args())
        NamedValues[std::string(Arg.getName())] = &Arg;

    fn.Body->accept(this);

    if (llvm::Value *RetVal = ValueStack.top()) {
        ValueStack.pop();
        // Finish off the function.
        Builder->CreateRet(RetVal);

        if(fn.IsAnonymous){
            TheFunction->addFnAttr(ANON);
        }
        // Validate the generated code, checking for consistency.
        verifyFunction(*TheFunction);
        OutputFunction = TheFunction;
        return;
    }

    // Error reading body, remove function.
    TheFunction->eraseFromParent();
    OutputFunction = nullptr;
}
}  // namespace kaleidoscope
