#include <Ast.h>
#include <Parser.h>

#include <catch2/catch.hpp>

namespace mathy::test {

namespace {

class ScriptedLexer : public ILexer {
public:
  explicit ScriptedLexer(std::vector<Token> &&tokens)
      : tokens(std::move(tokens)), pos(0) {}
  virtual ~ScriptedLexer() = default;
  // ILexer impl.
  Token lex() override {
    if (pos >= tokens.size())
      return Token(TokenKind::EndOfFile);
    return tokens.at(pos++);
  }

private:
  const std::vector<Token> tokens;
  size_t pos;
};

std::unique_ptr<IAst> createBinOp(char op, int lhs, int rhs) {
  return std::make_unique<BinOp>(op, std::make_unique<Number>(lhs),
                                 std::make_unique<Number>(rhs));
}

} // namespace

TEST_CASE("parser handles addition") { REQUIRE(true); }

} // namespace mathy::test
