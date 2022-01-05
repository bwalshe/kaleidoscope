#ifndef _KALEIDISCOPE_PARSER_HPP_
#define _KALEIDISCOPE_PARSER_HPP_

#include <memory>
#include <map>
#include <utility>

#include "Lexer.hpp"
#include "AST.hpp"

namespace kaleidiscope {

class Parser {
    std::unique_ptr<Lexer> ActiveLexer;
    int CurTok;
    int getNextToken() { return CurTok = ActiveLexer->gettok(); }


    /// BinopPrecedence - This holds the precedence for each binary operator
    /// that is defined.
    std::map<char, int> BinopPrecedence;

    /// GetTokPrecedence - Get the precedence of the pending binary operator
    /// token.
    int GetTokPrecedence();

    /// LogError* - These are little helper functions for error handling.
    std::unique_ptr<ExprAST> LogError(const char *Str);

    std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);

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
        : ActiveLexer(std::move(lexer)) {
        BinopPrecedence['<'] = 10;
        BinopPrecedence['+'] = 20;
        BinopPrecedence['-'] = 20;
        BinopPrecedence['*'] = 40;
        CurTok = ActiveLexer->gettok();
    }

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
};

}  // end namespace kaleidiscope
#endif  // _KALEIDOISCOPE_PARSER_HPP_

