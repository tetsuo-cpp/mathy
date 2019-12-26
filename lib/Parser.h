#pragma once

#include "Interfaces.h"

namespace mathy {

class Parser : public IParser {
public:
  explicit Parser(ILexer &lexer);
  virtual ~Parser() = default;
  // IParser impl.
  std::unique_ptr<IAst> parse() override;

private:
  std::unique_ptr<IAst> parseVar();
  std::unique_ptr<IAst> parseExpr();
  std::unique_ptr<IAst> parseAddition();
  std::unique_ptr<IAst> parseMultiplication();
  std::unique_ptr<IAst> parsePrimaryExpr();
  void readToken();
  bool checkToken(TokenKind);
  void expectToken(TokenKind);
  ILexer &lexer;
  Token curTok;
};

} // namespace mathy
