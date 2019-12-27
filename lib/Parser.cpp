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
  auto identifier = curTok.value;
  expectToken(TokenKind::Identifier);
  expectToken(TokenKind::Assignment);
  return std::make_unique<VarDecl>(std::move(identifier), parseExpr());
}

std::unique_ptr<IAst> Parser::parseExpr() { return parseAddition(); }

std::unique_ptr<IAst> Parser::parseAddition() {
  auto lhs = parseMultiplication();
  for (;;) {
    const auto prevTok = curTok;
    if (checkToken(TokenKind::Addition) || checkToken(TokenKind::Subtraction))
      lhs = std::make_unique<BinOp>(prevTok.kind, std::move(lhs),
                                    parseMultiplication());
    else
      return lhs;
  }
}

std::unique_ptr<IAst> Parser::parseMultiplication() {
  auto lhs = parsePrimaryExpr();
  for (;;) {
    const auto prevTok = curTok;
    if (checkToken(TokenKind::Multiplication) ||
        checkToken(TokenKind::Division))
      lhs = std::make_unique<BinOp>(prevTok.kind, std::move(lhs),
                                    parsePrimaryExpr());
    else
      return lhs;
  }
}

std::unique_ptr<IAst> Parser::parsePrimaryExpr() {
  const auto prevTok = curTok;
  if (checkToken(TokenKind::Number)) {
    const int intValue = std::stoi(prevTok.value);
    return std::make_unique<Number>(intValue);
  }
  expectToken(TokenKind::Identifier);
  return std::make_unique<VarRef>(prevTok.value);
}

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
