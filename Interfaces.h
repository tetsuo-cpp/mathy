#pragma once

#include <memory>
#include <optional>
#include <string>

namespace mathy {

enum class TokenKind {
  Identifier,
  Number,
  OpenParen,
  CloseParen,
  Addition,
  Subtraction,
  Multiplication,
  Division,
  Var,
  Assignment,
  EndOfFile
};

inline const char *tokenKindToString(TokenKind kind) {
  switch (kind) {
  case TokenKind::Identifier:
    return "Identifier";
  case TokenKind::Number:
    return "Number";
  case TokenKind::OpenParen:
    return "OpenParen";
  case TokenKind::CloseParen:
    return "CloseParen";
  case TokenKind::Addition:
    return "Addition";
  case TokenKind::Subtraction:
    return "Subtraction";
  case TokenKind::Multiplication:
    return "Multiplication";
  case TokenKind::Division:
    return "Division";
  case TokenKind::Var:
    return "Var";
  case TokenKind::Assignment:
    return "Assignment";
  case TokenKind::EndOfFile:
    return "EndOfFile";
  }
}

struct Token {
  explicit Token(TokenKind kind) : kind(kind) {}
  template <typename T>
  Token(TokenKind kind, T &&value)
      : kind(kind), value(std::forward<T>(value)) {}
  explicit operator bool() const { return (kind != TokenKind::EndOfFile); }
  TokenKind kind;
  const std::string value;
};

template <typename T> T &operator<<(T &stream, const Token &tok) {
  stream << "{ Kind = " << tokenKindToString(tok.kind) << ", Value = \""
         << tok.value << "\" }";
  return stream;
};

class IAst {
public:
  virtual ~IAst() = default;
};

class LexerError : public std::runtime_error {
public:
  template <typename T>
  explicit LexerError(T &&msg) : std::runtime_error(std::forward<T>(msg)) {}
  virtual ~LexerError() = default;
};

class ILexer {
public:
  virtual ~ILexer() = default;
  virtual Token lex() = 0;
};

class IParser {
public:
  virtual ~IParser() = default;
  virtual std::unique_ptr<IAst> parse() = 0;
};

class IEngine {
public:
  virtual ~IEngine() = default;
  virtual std::optional<int> eval() = 0;
};

} // namespace mathy
