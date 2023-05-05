#ifndef SAPPHIRE_AST_NODE_EXPRESSION_USEEXPRESSION_H_
#define SAPPHIRE_AST_NODE_EXPRESSION_USEEXPRESSION_H_
#include "Expression.h"

#include "ast/ast.h"
namespace ast {
namespace node {
class UseExpression : public Expression {
private:
  symbol::Symbol* symbol_;

public:
  UseExpression(long line, symbol::Symbol* symbol);
  UseExpression(symbol::Symbol* symbol);
  virtual ~UseExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  // virtual ASTNode* clone() override;

  CONST_MEMBER_FUNC(symbol::Symbol*, symbol)

  virtual Type* type() override;

  void setSymbol(symbol::Symbol* symbol);
  virtual void setType(Type* type) override;
};
} // namespace node
} // namespace ast
#endif
