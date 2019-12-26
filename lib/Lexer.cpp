#include "Lexer.h"

#include <sstream>

namespace mathy {

Lexer::Lexer(const std::string &source) : source(source), pos(0) { readChar(); }

Token Lexer::lex() {
  trimWhitespace();
  // Hit the end of the source buffer.
  if (curChar == 0)
    return Token(TokenKind::EndOfFile);
  if (::isdigit(curChar))
    return lexNumber();
  else if (::isalpha(curChar))
    return lexIdentifier();
  else
    return lexSymbol();
}

bool Lexer::readChar() {
  if (pos >= source.size()) {
    curChar = 0;
    return false;
  }
  curChar = source.at(pos++);
  return true;
}

void Lexer::trimWhitespace() {
  while (::isspace(curChar) && readChar())
    ;
}

Token Lexer::lexIdentifier() {
  std::string identifier{curChar};
  while (readChar() && ::isalnum(curChar))
    identifier.push_back(curChar);
  if (identifier == "var")
    return Token(TokenKind::Var);
  return Token(TokenKind::Identifier, std::move(identifier));
}

Token Lexer::lexNumber() {
  std::string number{curChar};
  while (readChar() && ::isdigit(curChar))
    number.push_back(curChar);
  return Token(TokenKind::Number, std::move(number));
}

Token Lexer::lexSymbol() {
  const auto prevChar = curChar;
  readChar();
  switch (prevChar) {
  case '+':
    return Token(TokenKind::Addition);
  case '-':
    return Token(TokenKind::Subtraction);
  case '*':
    return Token(TokenKind::Multiplication);
  case '/':
    return Token(TokenKind::Division);
  case '(':
    return Token(TokenKind::OpenParen);
  case ')':
    return Token(TokenKind::CloseParen);
  case '=':
    return Token(TokenKind::Assignment);
  default: {
    std::stringstream ss;
    ss << "Unrecognised symbol: \"" << prevChar << "\"";
    throw LexerError(ss.str());
  }
  }
}

} // namespace mathy
