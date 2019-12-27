#include <Interpreter.h>
#include <Lexer.h>
#include <Parser.h>

#include <iostream>

namespace {

const char *MATHY_PROMPT = "MATHY> ";

} // namespace

int main(int, char **) {
  mathy::Interpreter interpreter;
  std::string line;
  std::cout << MATHY_PROMPT;
  while (std::getline(std::cin, line)) {
    mathy::Lexer lexer(line);
    mathy::Parser parser(lexer);
    try {
      const auto value = interpreter.eval(*parser.parse());
      if (value)
        std::cout << *value << "\n";
      else
        std::cout << "NONE\n";
    } catch (const mathy::LexerError &error) {
      std::cout << "LEXER ERROR: " << error.what() << "\n";
    } catch (const mathy::ParserError &error) {
      std::cout << "PARSER ERROR: " << error.what() << "\n";
    } catch (const mathy::EngineError &error) {
      std::cout << "ENGINE ERROR: " << error.what() << "\n";
    }
    std::cout << MATHY_PROMPT;
  }
  return (0);
}
