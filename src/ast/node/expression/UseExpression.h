#ifndef SAPPHIRE_AST_NODE_USEEXPRESSION_H_
#define SAPPHIRE_AST_NODE_USEEXPRESSION_H_
#include "Expression.h"

#include "ast/ast.h"
namespace ast {
namespace node {
class UseExpression : public Expression {
private:
  symbol::Symbol* symbol_;

public:
  UseExpression(long lineNumber, symbol::Symbol* symbol)
      : UseExpression(symbol) {
    setLine(lineNumber);
  }
  UseExpression(symbol::Symbol* symbol) : symbol_(symbol) {}
  virtual ~UseExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  symbol::Symbol* symbol() { return symbol_; }
  virtual Type* type() override;
  virtual void setType(Type* type) override;
  void setSymbol(symbol::Symbol* symbol) { symbol_ = symbol; }
};
} // namespace node
} // namespace ast
#endif
