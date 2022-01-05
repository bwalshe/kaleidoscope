#include <catch2/catch.hpp>

#include "Lexer.hpp"


TEST_CASE("Lexer reads int") {
    kaleidiscope::MemoryLexer lexer(" 1000 ");
    REQUIRE(lexer.gettok() == kaleidiscope::tok_number);
    REQUIRE(lexer.getNumVal() == 1000);
}

TEST_CASE("Lexer reads def") {
    kaleidiscope::MemoryLexer lexer(" def ");
    REQUIRE(lexer.gettok() == kaleidiscope::tok_def);
}

TEST_CASE("Lexer reads extern") {
    kaleidiscope::MemoryLexer lexer(" extern ");
    REQUIRE(lexer.gettok() == kaleidiscope::tok_extern);
}

TEST_CASE("Lexer reads id") {
    kaleidiscope::MemoryLexer lexer(" foo ");
    REQUIRE(lexer.gettok() == kaleidiscope::tok_identifier);
    REQUIRE(lexer.getIdentifierStr() == "foo");
}

TEST_CASE("Lexer reads multiple tokens") {
    kaleidiscope::MemoryLexer lexer(" def foo extern 1");
    REQUIRE(lexer.gettok() == kaleidiscope::tok_def);
    REQUIRE(lexer.gettok() == kaleidiscope::tok_identifier);
    REQUIRE(lexer.getIdentifierStr() == "foo");
    REQUIRE(lexer.gettok() == kaleidiscope::tok_extern);
    REQUIRE(lexer.gettok() == kaleidiscope::tok_number);
    REQUIRE(lexer.getNumVal() == 1);
}

