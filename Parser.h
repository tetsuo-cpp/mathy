#pragma once

#include "Interfaces.h"

namespace mathy {

class Parser : public IParser {
public:
  Parser(ILexer &lexer);
  virtual ~Parser() = default;
  // IParser impl.
  std::unique_ptr<IAst> parse() override;

private:
  std::unique_ptr<IAst> parseVar();
  std::unique_ptr<IAst> parseExpr();
  void readToken();
  bool checkToken(TokenKind);
  void expectToken(TokenKind);
  ILexer &lexer;
  Token curTok;
};

} // namespace mathy
