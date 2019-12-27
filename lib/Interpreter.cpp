#include "Interpreter.h"
#include <sstream>

namespace mathy {

std::optional<int> Interpreter::eval(IAst &node) {
  node.accept(*this);
  return node.value;
}

void Interpreter::visit(VarDecl &varDecl) {
  varDecl.rhs->accept(*this);
  assert(varDecl.rhs->value);
  bindings[varDecl.name] = *varDecl.rhs->value;
}

void Interpreter::visit(BinOp &binOp) {
  binOp.lhs->accept(*this);
  assert(binOp.lhs->value);
  binOp.rhs->accept(*this);
  assert(binOp.rhs->value);
  binOp.value = [&binOp]() -> std::optional<int> {
    switch (binOp.op) {
    case TokenKind::Addition:
      return *binOp.lhs->value + *binOp.rhs->value;
    case TokenKind ::Subtraction:
      return *binOp.lhs->value - *binOp.rhs->value;
    case TokenKind ::Multiplication:
      return *binOp.lhs->value * *binOp.rhs->value;
    case TokenKind ::Division:
      return *binOp.lhs->value / *binOp.rhs->value;
    default: {
      std::stringstream ss;
      ss << "Encountered bin op with operator of kind "
         << tokenKindToString(binOp.op);
      throw EngineError(ss.str());
    }
    }
  }();
}

void Interpreter::visit(Number &number) {
  // Oops, called them the same thing. Oh well...
  number.IAst::value = number.Number::value;
}

void Interpreter::visit(VarRef &varRef) {
  const auto iter = bindings.find(varRef.name);
  if (iter == bindings.end()) {
    std::stringstream ss;
    ss << "Encountered ref to unrecognised var " << varRef.name;
    throw EngineError(ss.str());
  }
  varRef.value = iter->second;
}

} // namespace mathy
