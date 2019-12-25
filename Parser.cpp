#include "Parser.h"

#include "Ast.h"

#include <sstream>

namespace mathy {

Parser::Parser(ILexer &lexer) : lexer(lexer), curTok(TokenKind::EndOfFile) {
  readToken();
}

std::unique_ptr<IAst> Parser::parse() {
  if (!curTok)
    return nullptr;
  if (checkToken(TokenKind::Var))
    return parseVar();
  return parseExpr();
}

std::unique_ptr<IAst> Parser::parseVar() {
  // Parsing form: var x = 1 + 2.
  std::string identifier = curTok.value;
  expectToken(TokenKind::Identifier);
  expectToken(TokenKind::Assignment);
  return std::make_unique<VarDecl>(std::move(identifier), parseExpr());
}

std::unique_ptr<IAst> Parser::parseExpr() { return nullptr; }

void Parser::readToken() { curTok = lexer.lex(); }

bool Parser::checkToken(TokenKind kind) {
  if (kind != curTok.kind)
    return false;
  readToken();
  return true;
}

void Parser::expectToken(TokenKind kind) {
  if (!checkToken(kind)) {
    std::stringstream ss;
    ss << "Expected token of type " << tokenKindToString(kind) << " but got "
       << tokenKindToString(curTok.kind);
    throw ParserError(ss.str());
  }
}

} // namespace mathy
