#include <Parser.h>

#include <catch2/catch.hpp>

namespace mathy::test {

class ScriptedLexer : public ILexer {
public:
  ScriptedLexer(std::vector<Token> &&tokens)
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

TEST_CASE("parser test") { REQUIRE(true); }

} // namespace mathy::test
