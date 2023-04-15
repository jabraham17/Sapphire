#ifndef SAPPHIRE_AST_NODE_UINTEXPRESSION_H_
#define SAPPHIRE_AST_NODE_UINTEXPRESSION_H_
#include "ast/ast.h"
#include "ast/node/expression/Expression.h"

namespace ast {
namespace node {
class UIntExpression : public Expression {
private:
  using value_type = unsigned long long;
  value_type value_;

public:
  UIntExpression(long lineNumber, value_type value);
  UIntExpression(value_type value);
  virtual ~UIntExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  virtual Type* type() override;
  virtual void setType(Type* type) override;

  value_type value();
};
} // namespace node
} // namespace ast
#endif
