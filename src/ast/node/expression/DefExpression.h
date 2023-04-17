#ifndef SAPPHIRE_AST_NODE_EXPRESSION_DEFEXPRESSION_H_
#define SAPPHIRE_AST_NODE_EXPRESSION_DEFEXPRESSION_H_
#include "Expression.h"

#include "ast/ast.h"
namespace ast {
namespace node {

class DefExpression : public Expression {
  symbol::Symbol* symbol_;
  Expression* initialValue_;

protected:
  virtual void replaceNode(ASTNode* old, ASTNode* replacement) override;

public:
  DefExpression(
      long line,
      Type* type,
      symbol::Symbol* symbol,
      Expression* initialValue = nullptr);
  DefExpression(
      long line,
      symbol::Symbol* symbol,
      Expression* initialValue = nullptr);
  DefExpression(
      Type* type,
      symbol::Symbol* symbol,
      Expression* initialValue = nullptr);
  DefExpression(symbol::Symbol* symbol, Expression* initialValue = nullptr);

  virtual ~DefExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  symbol::Symbol* symbol();

  virtual Type* type() override;
  virtual void setType(Type* type) override;

  void setInitialValue(Expression* value);
  bool hasInitialValue();
  Expression* initialValue();
};
} // namespace node
} // namespace ast
#endif
