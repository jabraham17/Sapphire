#ifndef SAPPHIRE_AST_NODE_EXPRESSION_DEFEXPRESSION_H_
#define SAPPHIRE_AST_NODE_EXPRESSION_DEFEXPRESSION_H_
#include "ast/ast.h"
#include "expression/Expression.h"
namespace ast {
namespace node {

class DefExpression : public Expression {
  symbol::Symbol* symbol_;
  Expression* assignValue_;

public:
  DefExpression(
      long lineNumber,
      Type* type,
      symbol::Symbol* symbol,
      Expression* assignValue)
      : DefExpression(type, symbol, assignValue) {
    setLine(lineNumber);
  }
  DefExpression(Type* type, symbol::Symbol* symbol, Expression* assignValue);
  DefExpression(long lineNumber, Type* type, symbol::Symbol* symbol)
      : DefExpression(type, symbol) {
    setLine(lineNumber);
  }
  DefExpression(Type* type, symbol::Symbol* symbol);
  DefExpression(
      long lineNumber,
      symbol::Symbol* symbol,
      Expression* assignValue)
      : DefExpression(symbol, assignValue) {
    setLine(lineNumber);
  }
  DefExpression(symbol::Symbol* symbol, Expression* assignValue);
  DefExpression(long lineNumber, symbol::Symbol* symbol)
      : DefExpression(symbol) {
    setLine(lineNumber);
  }
  DefExpression(symbol::Symbol* symbol);
  virtual ~DefExpression() = default;
  virtual void setValue(Expression* value) { assignValue_ = value; }
  virtual void accept(visitor::ASTVisitor* ast) override;
  symbol::Symbol* symbol() { return symbol_; }
  virtual Type* type() override;
  virtual void setType(Type* type) override;
  bool hasInitialValue();
  Expression* assignValue() { return assignValue_; }
};
} // namespace node
} // namespace ast
#endif
