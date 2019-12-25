#include "Parser.h"

namespace mathy {

Parser::Parser(ILexer &lexer) : lexer(lexer) { static_cast<void>(this->lexer); }

std::unique_ptr<IAst> Parser::parse() { return std::make_unique<IAst>(); }

} // namespace mathy
