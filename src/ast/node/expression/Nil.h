#ifndef SAPPHIRE_AST_NODE_EXPRESSION_NIL_H_
#define SAPPHIRE_AST_NODE_EXPRESSION_NIL_H_
#include "Expression.h"

#include "ast/ast.h"

namespace ast {
namespace node {
class Nil : public Expression {
  bool isUserSpecified_;

public:
  Nil(long line);
  Nil();
  Nil(long line, bool isUserSpecified);
  Nil(bool isUserSpecified);
  virtual ~Nil() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual ASTNode* clone() override;

  virtual Type* type() override;
  virtual void setType(Type* type) override;

  bool isUserSpecified();
};
} // namespace node
} // namespace ast
#endif
