#pragma once

#include "Interfaces.h"

namespace mathy {

class Lexer : public ILexer {
public:
  explicit Lexer(const std::string &);
  virtual ~Lexer() = default;
  // ILexer impl.
  Token lex() override;

private:
  bool readChar();
  void trimWhitespace();
  Token lexNumber();
  Token lexIdentifier();
  Token lexSymbol();
  const std::string &source;
  size_t pos;
  char curChar;
};

} // namespace mathy
