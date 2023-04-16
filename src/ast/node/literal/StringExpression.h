#ifndef SAPPHIRE_AST_NODE_LITERAL_STRINGEXPRESSION_H_
#define SAPPHIRE_AST_NODE_LITERAL_STRINGEXPRESSION_H_
#include "ast/ast.h"
#include "ast/node/expression/Expression.h"

#include <string>

namespace ast {
namespace node {

class StringExpression : public Expression {
public:
  using value_type = std::string;

private:
  value_type value_;

public:
  StringExpression(long lineNumber, const value_type& value);
  StringExpression(const value_type& value);
  virtual ~StringExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  virtual Type* type() override;
  virtual void setType(Type* type) override;

  value_type value();
  //  return the string with all escapes resolved
  value_type escapedValue();
};

} // namespace node
} // namespace ast
#endif
