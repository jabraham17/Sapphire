#ifndef SAPPHIRE_AST_NODE_LITERAL_REALEXPRESSION_H_
#define SAPPHIRE_AST_NODE_LITERAL_REALEXPRESSION_H_
#include "ast/ast.h"
#include "ast/node/expression/Expression.h"

namespace ast {
namespace node {
class RealExpression : public Expression {
public:
  using value_type = double;

private:
  value_type value_;

public:
  RealExpression(long line, double value);
  RealExpression(double value);
  virtual ~RealExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  // virtual ASTNode* clone() override;

  virtual Type* type() override;
  virtual void setType(Type* type) override;

  CONST_MEMBER_FUNC(value_type, value);
};
} // namespace node
} // namespace ast
#endif
