#ifndef SAPPHIRE_AST_NODE_EXPRESSION_DEFEXPRESSION_H_
#define SAPPHIRE_AST_NODE_EXPRESSION_DEFEXPRESSION_H_
#include "Expression.h"

#include "ast/ast.h"
namespace ast {
namespace node {

class DefExpression : public Expression {
  symbol::Symbol* symbol_;
  std::ptrdiff_t initialValueIdx_;

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
  // virtual ASTNode* clone() override;

  CONST_MEMBER_FUNC(symbol::Symbol*, symbol)

  virtual Type* type() override;
  virtual void setType(Type* type) override;

  void setInitialValue(Expression* value);
  CONST_MEMBER_FUNC(bool, hasInitialValue);
  CONST_MEMBER_FUNC(Expression*, initialValue);
};
} // namespace node
} // namespace ast
#endif
