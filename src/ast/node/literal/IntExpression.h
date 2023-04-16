#ifndef SAPPHIRE_AST_NODE_LITERAL_INTEXPRESSION_H_
#define SAPPHIRE_AST_NODE_LITERAL_INTEXPRESSION_H_
#include "ast/ast.h"
#include "ast/node/expression/Expression.h"

namespace ast {
namespace node {

class IntExpression : public Expression {
public:
  using value_type = long long;

private:
  value_type value_;

public:
  IntExpression(long lineNumber, value_type value);
  IntExpression(value_type value);
  virtual ~IntExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  virtual Type* type() override;
  virtual void setType(Type* type) override;

  value_type value();
};
} // namespace node
} // namespace ast
#endif
