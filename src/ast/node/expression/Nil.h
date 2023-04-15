#ifndef SAPPHIRE_AST_NODE_EXPRESSION_NIL_H_
#define SAPPHIRE_AST_NODE_EXPRESSION_NIL_H_
#include "ast/ast.h"
#include "expression/Expression.h"
namespace ast {
namespace node {
class Nil : public Expression {
  bool isUserSpecified_;

public:
  Nil(long lineNumber) : Nil() { setLine(lineNumber); }
  Nil() : Nil(false) {}
  Nil(long lineNumber, bool isUserSpecified) : Nil(isUserSpecified) {
    setLine(lineNumber);
  }
  Nil(bool isUserSpecified) : isUserSpecified_(isUserSpecified) {}
  virtual ~Nil() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual Type* type() override { return Type::getNilType(); }
  virtual void setType(Type* type) override {
    assert(false && "cannot set the type of nil");
  }
  bool isUserSpecified() { return isUserSpecified_; }
};
} // namespace node
} // namespace ast
#endif
