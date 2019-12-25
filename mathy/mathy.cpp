#include <Interfaces.h>
#include <Lexer.h>

#include <iostream>
#include <vector>

namespace {

void lex(mathy::ILexer &lexer, std::vector<mathy::Token> &tokens,
         bool verbose) {
  while (auto tok = lexer.lex()) {
    if (verbose)
      std::cout << tok << "\n";
    tokens.push_back(std::move(tok));
  }
}

} // namespace

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  const std::string source("1 + 3");
  mathy::Lexer lexer(source);
  std::vector<mathy::Token> tokens;
  try {
    lex(lexer, tokens, true);
  } catch (const mathy::LexerError &error) {
    std::cout << error.what() << "\n";
  }
  return (0);
}
