#include <Lexer.h>

#include <catch2/catch.hpp>

namespace mathy::test {

namespace {

void testLexer(const std::string &source, const std::vector<Token> &expected) {
  std::vector<Token> actual;
  Lexer lexer(source);
  while (auto tok = lexer.lex())
    actual.push_back(std::move(tok));
  REQUIRE(expected == actual);
}

} // namespace

TEST_CASE("lexer handles addition", "[lexer]") {
  testLexer("1 + 3", {Token(TokenKind::Number, "1"), Token(TokenKind::Addition),
                      Token(TokenKind::Number, "3")});
}

TEST_CASE("lexer handles subtraction", "[lexer]") {
  testLexer("1 - 3",
            {Token(TokenKind::Number, "1"), Token(TokenKind::Subtraction),
             Token(TokenKind::Number, "3")});
}

TEST_CASE("lexer handles multiplication", "[lexer]") {
  testLexer("3 * 2",
            {Token(TokenKind::Number, "3"), Token(TokenKind::Multiplication),
             Token(TokenKind::Number, "2")});
}

TEST_CASE("lexer handles division", "[lexer]") {
  testLexer("4 / 2", {Token(TokenKind::Number, "4"), Token(TokenKind::Division),
                      Token(TokenKind::Number, "2")});
}

TEST_CASE("lexer handles var decl", "[lexer]") {
  testLexer("var foo = 1",
            {Token(TokenKind::Var), Token(TokenKind::Identifier, "foo"),
             Token(TokenKind::Assignment), Token(TokenKind::Number, "1")});
}

TEST_CASE("lexer handles parens", "[lexer]") {
  testLexer("(1 + 3) * 2",
            {Token(TokenKind::OpenParen), Token(TokenKind::Number, "1"),
             Token(TokenKind::Addition), Token(TokenKind::Number, "3"),
             Token(TokenKind::CloseParen), Token(TokenKind::Multiplication),
             Token(TokenKind::Number, "2")});
}

TEST_CASE("lexer handles exprs without spacing", "[lexer]") {
  testLexer("1+3*x-2",
            {Token(TokenKind::Number, "1"), Token(TokenKind::Addition),
             Token(TokenKind::Number, "3"), Token(TokenKind::Multiplication),
             Token(TokenKind::Identifier, "x"), Token(TokenKind::Subtraction),
             Token(TokenKind::Number, "2")});
}

} // namespace mathy::test
