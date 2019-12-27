#pragma once

#include "Ast.h"
#include "Interfaces.h"

#include <unordered_map>

namespace mathy {

class Interpreter : public IEngine, public IAstVisitor {
public:
  virtual ~Interpreter() = default;
  // IEngine impl.
  std::optional<int> eval(IAst &) override;

private:
  // IAstVisitor impl.
  void visit(VarDecl &) override;
  void visit(BinOp &) override;
  void visit(Number &) override;
  void visit(VarRef &) override;
  std::unordered_map<std::string, int> bindings;
};

} // namespace mathy
