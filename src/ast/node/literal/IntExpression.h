#ifndef SAPPHIRE_AST_NODE_INTEXPRESSION_H_
#define SAPPHIRE_AST_NODE_INTEXPRESSION_H_
#include "ast/ast.h"
#include "ast/node/expression/Expression.h"
namespace ast {
namespace node {

class IntExpression : public Expression {
private:
  long long value_;

public:
  IntExpression(long lineNumber, long long value) : IntExpression(value) {
    setLine(lineNumber);
  }
  IntExpression(long long value) : value_(value) {}
  virtual ~IntExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual Type* type() override {
    return new PrimitiveType(PrimitiveTypeEnum::INT);
  }
  virtual void setType(Type* type) override {
    assert(false && "cannot set the type of an int expression");
  }
  auto value() { return value_; }
};
} // namespace node
} // namespace ast
#endif
