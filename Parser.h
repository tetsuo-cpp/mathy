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
  ILexer &lexer;
};

} // namespace mathy
