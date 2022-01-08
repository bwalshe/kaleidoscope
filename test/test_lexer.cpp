#include <catch2/catch.hpp>

#include "Lexer.hpp"


TEST_CASE("Lexer reads int") {
    kaleidoscope::MemoryLexer lexer(" 1000 ");
    REQUIRE(lexer.gettok() == kaleidoscope::tok_number);
    REQUIRE(lexer.getNumVal() == 1000);
}

TEST_CASE("Lexer reads def") {
    kaleidoscope::MemoryLexer lexer(" def ");
    REQUIRE(lexer.gettok() == kaleidoscope::tok_def);
}

TEST_CASE("Lexer reads extern") {
    kaleidoscope::MemoryLexer lexer(" extern ");
    REQUIRE(lexer.gettok() == kaleidoscope::tok_extern);
}

TEST_CASE("Lexer reads id") {
    kaleidoscope::MemoryLexer lexer(" foo ");
    REQUIRE(lexer.gettok() == kaleidoscope::tok_identifier);
    REQUIRE(lexer.getIdentifierStr() == "foo");
}

TEST_CASE("Lexer reads multiple tokens") {
    kaleidoscope::MemoryLexer lexer(" def foo extern 1");
    REQUIRE(lexer.gettok() == kaleidoscope::tok_def);
    REQUIRE(lexer.gettok() == kaleidoscope::tok_identifier);
    REQUIRE(lexer.getIdentifierStr() == "foo");
    REQUIRE(lexer.gettok() == kaleidoscope::tok_extern);
    REQUIRE(lexer.gettok() == kaleidoscope::tok_number);
    REQUIRE(lexer.getNumVal() == 1);
}

