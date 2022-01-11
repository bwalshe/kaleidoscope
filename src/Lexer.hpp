#ifndef _KALEIDOSCOPE_LEXER_HPP_
#define _KALEIDOSCOPE_LEXER_HPP_

#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>


namespace kaleidoscope {

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5,

  tok_uninitialised = -6
};

class Lexer {
    std::string IdentifierStr;  // Filled in if tok_identifier
    double NumVal;              // Filled in if tok_number
    int LastChar;

 public:
    Lexer() : LastChar(' ') {}

    virtual ~Lexer() = default;

    virtual char nextChar() = 0;

    /// gettok - Return the next token from standard input.
    int gettok();

    std::string const &getIdentifierStr() const {
        return IdentifierStr;
    }

    double getNumVal() {
        return NumVal;
    }
};

class StdInLexer : public Lexer {
    char nextChar() {
        return getchar();
    }
};

class MemoryLexer : public Lexer {
    std::istringstream In;

 public:
    explicit MemoryLexer(const std::string &text): In(text) {}

    char nextChar() {
        char c{std::char_traits<char>::eof()};
        In.get(c);
        return c;
    }
};

class FileLexer : public Lexer {
    std::ifstream In;

 public:
    explicit FileLexer(const std::string &fileName) : In(fileName) {}


    ~FileLexer() {
        In.close();
    }


    char nextChar() {
        char c;
        In.get(c);
        return c;
    }
};

}  // end namespace kaleidoscope
#endif  // _KALEIDOSCOPE_LEXER_HPP_

