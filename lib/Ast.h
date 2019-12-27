#pragma once

#include "Interfaces.h"

#include <memory>
#include <string>

namespace mathy {

struct VarDecl : public IAst {
  VarDecl(const std::string &name, std::unique_ptr<IAst> rhs)
      : name(name), rhs(std::move(rhs)) {}
  virtual ~VarDecl() = default;
  // IAst impl.
  void accept(IAstVisitor &) override;
  const std::string name;
  std::unique_ptr<IAst> rhs;
};

struct BinOp : public IAst {
  BinOp(TokenKind op, std::unique_ptr<IAst> lhs, std::unique_ptr<IAst> rhs)
      : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
  virtual ~BinOp() = default;
  // IAst impl.
  void accept(IAstVisitor &) override;
  const TokenKind op;
  std::unique_ptr<IAst> lhs;
  std::unique_ptr<IAst> rhs;
};

struct Number : public IAst {
  explicit Number(int value) : value(value) {}
  virtual ~Number() = default;
  // IAst impl.
  void accept(IAstVisitor &) override;
  const int value;
};

struct VarRef : public IAst {
  explicit VarRef(const std::string &name) : name(name) {}
  virtual ~VarRef() = default;
  // IAst impl.
  void accept(IAstVisitor &) override;
  const std::string name;
};

class IAstVisitor {
public:
  virtual ~IAstVisitor() = default;
  virtual void visit(VarDecl &) = 0;
  virtual void visit(BinOp &) = 0;
  virtual void visit(Number &) = 0;
  virtual void visit(VarRef &) = 0;
};

inline void VarDecl::accept(IAstVisitor &visitor) { visitor.visit(*this); }

inline void BinOp::accept(IAstVisitor &visitor) { visitor.visit(*this); }

inline void Number::accept(IAstVisitor &visitor) { visitor.visit(*this); }

inline void VarRef::accept(IAstVisitor &visitor) { visitor.visit(*this); }

} // namespace mathy
