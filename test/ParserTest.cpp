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

#define COMPARE_HELPER(type, variable)                                         \
  const auto *variable = dynamic_cast<const type *>(&lhs);                     \
  if (variable) {                                                              \
    const auto *variable##Rhs = dynamic_cast<const type *>(&rhs);              \
    INFO("checking that rhs is of type " #type);                               \
    REQUIRE(variable##Rhs);                                                    \
    return compareImpl(*variable, *variable##Rhs);                             \
  }

void compare(const IAst &lhs, const IAst &rhs) {
  COMPARE_HELPER(VarDecl, varDecl);
  COMPARE_HELPER(BinOp, binOp);
  COMPARE_HELPER(Number, number);
  // Means we haven't added a helper for the type of lhs.
  INFO("encountered unexpected ast type");
  REQUIRE(false);
}

std::unique_ptr<IAst> createBinOp(char op, int lhs, int rhs) {
  return std::make_unique<BinOp>(op, std::make_unique<Number>(lhs),
                                 std::make_unique<Number>(rhs));
}

} // namespace

TEST_CASE("parser handles addition") {
  auto var = createBinOp('+', 1, 2);
  static_cast<void>(var);
  compare(Number(1), Number(1));
  compare(*var, Number(1));
}

} // namespace mathy::test
