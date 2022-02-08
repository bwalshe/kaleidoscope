#ifndef _KALEIDOSCOPE_PARSER_HPP_
#define _KALEIDOSCOPE_PARSER_HPP_

#include <memory>
#include <map>
#include <utility>
#include <exception>

#include "Lexer.hpp"
#include "AST.hpp"

namespace kaleidoscope {

class ParseException : public std::exception {
    const char* Msg;
 public:
     ParseException(const char* msg) : Msg(msg) {}

     const char* what() const throw() {
         return Msg;
     }
};

class Parser {
    std::unique_ptr<Lexer> ActiveLexer;
    int CurTok;
    int getNextToken() { return CurTok = ActiveLexer->gettok(); }

    /// identifierexpr
    ///   ::= identifier
    ///   ::= identifier '(' expression* ')'
    std::unique_ptr<ExprAST> ParseIdentifierExpr();

    /// primary
    ///   ::= identifierexpr
    ///   ::= numberexpr
    ///   ::= parenexpr
    std::unique_ptr<ExprAST> ParsePrimary();

    /// expression
    ///   ::= primary binoprhs
    ///
    std::unique_ptr<ExprAST> ParseExpression();

    /// prototype
    ///   ::= id '(' id* ')'
    std::unique_ptr<PrototypeAST> ParsePrototype();

    /// definition ::= 'def' prototype expression
    std::unique_ptr<FunctionAST> ParseDefinition();

    /// toplevelexpr ::= expression
    std::unique_ptr<FunctionAST> ParseTopLevelExpr();

    /// external ::= 'extern' prototype
    std::unique_ptr<PrototypeAST> ParseExtern();

    /// BinopPrecedence - This holds the precedence for each binary operator
    /// that is defined.
    std::map<char, int> BinopPrecedence;

    /// GetTokPrecedence - Get the precedence of the pending binary operator
    /// token.
    int GetTokPrecedence();

    /// numberexpr ::= number
    std::unique_ptr<ExprAST> ParseNumberExpr();

    /// parenexpr ::= '(' expression ')'
    std::unique_ptr<ExprAST> ParseParenExpr();

    /// binoprhs
    ///   ::= ('+' primary)*
    std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                                  std::unique_ptr<ExprAST> LHS);


 public:
    static constexpr const char* ANON_FUNCTION_NAME = "__anon_expr";

    explicit Parser(std::unique_ptr<Lexer> lexer)
        : ActiveLexer(std::move(lexer)), CurTok(tok_uninitialised) {
        BinopPrecedence['<'] = 10;
        BinopPrecedence['+'] = 20;
        BinopPrecedence['-'] = 20;
        BinopPrecedence['*'] = 40;
        //CurTok = ActiveLexer->gettok();
    }


    /// get the hext non-trivial top level node where
    /// top ::= definition | external | expression | ;
    std::unique_ptr<ASTNode> NextTopLevelASTNode();
};

}  // end namespace kaleidoscope
#endif  // _KALEIDOISCOPE_PARSER_HPP_

