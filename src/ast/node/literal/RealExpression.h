#ifndef SAPPHIRE_AST_NODE_REALEXPRESSION_H_
#define SAPPHIRE_AST_NODE_REALEXPRESSION_H_
#include "ast/ast.h"
#include "ast/node/expression/Expression.h"
namespace ast {
namespace node {
class RealExpression : public Expression {
private:
  double value_;

public:
  RealExpression(long lineNumber, double value) : RealExpression(value) {
    setLine(lineNumber);
  }
  RealExpression(double value) : value_(value) {}
  virtual ~RealExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual Type* type() override {
    return new PrimitiveType(PrimitiveTypeEnum::REAL);
  }
  virtual void setType(Type* type) override {
    assert(false && "cannot set the type of a real expression");
  }
  auto value() { return value_; }
};
} // namespace node
} // namespace ast
#endif
