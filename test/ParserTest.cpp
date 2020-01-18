#include <Ast.h>
#include <Lexer.h>
#include <Parser.h>

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

namespace mathy::test {

namespace {

class ScriptedLexer : public ILexer {
public:
  explicit ScriptedLexer(const std::vector<Token> &tokens)
      : tokens(tokens), pos(0) {}
  virtual ~ScriptedLexer() = default;
  // ILexer impl.
  Token lex() override {
    if (pos >= tokens.size())
      return Token(TokenKind::EndOfFile);
    return tokens.at(pos++);
  }

private:
  const std::vector<Token> &tokens;
  size_t pos;
};

class MockLexer : public ILexer {
public:
  MAKE_MOCK0(lex, Token(), override);
};

void compare(const IAst &lhs, const IAst &rhs);

void compareImpl(const VarDecl &lhs, const VarDecl &rhs) {
  REQUIRE(lhs.name == rhs.name);
  compare(*lhs.rhs, *rhs.rhs);
}

void compareImpl(const BinOp &lhs, const BinOp &rhs) {
  REQUIRE(lhs.op == rhs.op);
  compare(*lhs.lhs, *rhs.lhs);
  compare(*lhs.rhs, *rhs.rhs);
}

void compareImpl(const Number &lhs, const Number &rhs) {
  REQUIRE(lhs.value == rhs.value);
}

void compareImpl(const VarRef &lhs, const VarRef &rhs) {
  REQUIRE(lhs.name == rhs.name);
}

#define COMPARE_HELPER(type, variable)                                         \
  const auto *variable = dynamic_cast<const type *>(&lhs);                     \
  if (variable) {                                                              \
    const auto *variable##Rhs = dynamic_cast<const type *>(&rhs);              \
    INFO("Checking that rhs is of type " #type);                               \
    REQUIRE(variable##Rhs);                                                    \
    return compareImpl(*variable, *variable##Rhs);                             \
  }

void compare(const IAst &lhs, const IAst &rhs) {
  COMPARE_HELPER(VarDecl, varDecl);
  COMPARE_HELPER(BinOp, binOp);
  COMPARE_HELPER(Number, number);
  COMPARE_HELPER(VarRef, varRef);
  // Means we haven't added a helper for the type of lhs.
  INFO("Encountered unexpected ast type");
  REQUIRE(false);
}

void testWithScriptedLexer(const std::vector<Token> &tokens,
                           const IAst &expected) {
  ScriptedLexer lexer(tokens);
  Parser parser(lexer);
  compare(expected, *parser.parse());
}

void testWithMockLexer(const std::vector<Token> &tokens, const IAst &expected) {
  trompeloeil::sequence seq;
  MockLexer lexer;
  Parser parser(lexer);
  for (size_t i = 0; i < tokens.size(); ++i)
    REQUIRE_CALL(lexer, lex()).RETURN(tokens.at(i)).IN_SEQUENCE(seq).TIMES(1);
  REQUIRE_CALL(lexer, lex())
      .RETURN(Token(TokenKind::EndOfFile))
      .IN_SEQUENCE(seq)
      .TIMES(1);
  compare(expected, *parser.parse());
}

void testWithLexer(const std::string &source, const IAst &expected) {
  Lexer lexer(source);
  Parser parser(lexer);
  compare(expected, *parser.parse());
}

void testParser(const std::string &source,
                const std::vector<Token> &scriptedTokens,
                const IAst &expected) {
  testWithScriptedLexer(scriptedTokens, expected);
  testWithMockLexer(scriptedTokens, expected);
  testWithLexer(source, expected);
}

} // namespace

TEST_CASE("parser handles addition", "[parser]") {
  testParser("1 + 2",
             {Token(TokenKind::Number, "1"), Token(TokenKind::Addition),
              Token(TokenKind::Number, "2")},
             BinOp(TokenKind::Addition, std::make_unique<Number>(1),
                   std::make_unique<Number>(2)));
}

TEST_CASE("parser handles subtraction", "[parser]") {
  testParser("8 - 1",
             {Token(TokenKind::Number, "8"), Token(TokenKind::Subtraction),
              Token(TokenKind::Number, "1")},
             BinOp(TokenKind::Subtraction, std::make_unique<Number>(8),
                   std::make_unique<Number>(1)));
}

TEST_CASE("parser handles multiplication", "[parser]") {
  testParser("2 * 5",
             {Token(TokenKind::Number, "2"), Token(TokenKind::Multiplication),
              Token(TokenKind::Number, "5")},
             BinOp(TokenKind::Multiplication, std::make_unique<Number>(2),
                   std::make_unique<Number>(5)));
}

TEST_CASE("parser handles division", "[parser]") {
  testParser("6 / 3",
             {Token(TokenKind::Number, "6"), Token(TokenKind::Division),
              Token(TokenKind::Number, "3")},
             BinOp(TokenKind::Division, std::make_unique<Number>(6),
                   std::make_unique<Number>(3)));
}

TEST_CASE("parser handles precedence", "[parser]") {
  testParser("3 + 2 * 1 / 2 + 3",
             {Token(TokenKind::Number, "3"), Token(TokenKind::Addition),
              Token(TokenKind::Number, "2"), Token(TokenKind::Multiplication),
              Token(TokenKind::Number, "1"), Token(TokenKind::Division),
              Token(TokenKind::Number, "2"), Token(TokenKind::Addition),
              Token(TokenKind::Number, "3")},
             BinOp(TokenKind::Addition,
                   std::make_unique<BinOp>(
                       TokenKind::Addition, std::make_unique<Number>(3),
                       std::make_unique<BinOp>(
                           TokenKind::Division,
                           std::make_unique<BinOp>(TokenKind::Multiplication,
                                                   std::make_unique<Number>(2),
                                                   std::make_unique<Number>(1)),
                           std::make_unique<Number>(2))),
                   std::make_unique<Number>(3)));
}

TEST_CASE("parser handles parens", "[parser]") {
  testParser("3 * (2 + 1)",
             {Token(TokenKind::Number, "3"), Token(TokenKind::Multiplication),
              Token(TokenKind::OpenParen), Token(TokenKind::Number, "2"),
              Token(TokenKind::Addition), Token(TokenKind::Number, "1"),
              Token(TokenKind::CloseParen)},
             BinOp(TokenKind::Multiplication, std::make_unique<Number>(3),
                   std::make_unique<BinOp>(TokenKind::Addition,
                                           std::make_unique<Number>(2),
                                           std::make_unique<Number>(1))));
}

TEST_CASE("parser handles var decls", "[parser]") {
  testParser(
      "var foo = 1 + 2",
      {Token(TokenKind::Var), Token(TokenKind::Identifier, "foo"),
       Token(TokenKind::Assignment), Token(TokenKind::Number, "1"),
       Token(TokenKind::Addition), Token(TokenKind::Number, "2")},
      VarDecl("foo", std::make_unique<BinOp>(TokenKind::Addition,
                                             std::make_unique<Number>(1),
                                             std::make_unique<Number>(2))));
}

TEST_CASE("parser handles identifiers", "[parser]") {
  testParser("1 + bar",
             {Token(TokenKind::Number, "1"), Token(TokenKind::Addition),
              Token(TokenKind::Identifier, "bar")},
             BinOp(TokenKind::Addition, std::make_unique<Number>(1),
                   std::make_unique<VarRef>("bar")));
}

} // namespace mathy::test
