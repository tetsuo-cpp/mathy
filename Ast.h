#pragma once

#include "Interfaces.h"

#include <memory>
#include <string>

namespace mathy {

struct VarDecl : public IAst {
  VarDecl(const std::string &name, std::unique_ptr<IAst> rhs)
      : name(name), rhs(std::move(rhs)) {}
  virtual ~VarDecl() = default;
  const std::string name;
  std::unique_ptr<IAst> rhs;
};

struct BinOp : public IAst {
  BinOp(char op, std::unique_ptr<IAst> lhs, std::unique_ptr<IAst> rhs)
      : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
  virtual ~BinOp() = default;
  const char op;
  std::unique_ptr<IAst> lhs;
  std::unique_ptr<IAst> rhs;
};

struct Number : public IAst {
  explicit Number(int value) : value(value) {}
  virtual ~Number() = default;
  const int value;
};

} // namespace mathy
