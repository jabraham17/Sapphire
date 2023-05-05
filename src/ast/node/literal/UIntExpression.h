#ifndef SAPPHIRE_AST_NODE_LITERAL_UINTEXPRESSION_H_
#define SAPPHIRE_AST_NODE_LITERAL_UINTEXPRESSION_H_

#include "ast/ast.h"
#include "ast/node/expression/Expression.h"

namespace ast {
namespace node {
class UIntExpression : public Expression {
public:
  using value_type = unsigned long long;

private:
  value_type value_;

public:
  UIntExpression(long line, value_type value);
  UIntExpression(value_type value);
  virtual ~UIntExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  // virtual ASTNode* clone() override;

  virtual Type* type() override;
  virtual void setType(Type* type) override;

  CONST_MEMBER_FUNC(value_type, value);
};
} // namespace node
} // namespace ast
#endif
