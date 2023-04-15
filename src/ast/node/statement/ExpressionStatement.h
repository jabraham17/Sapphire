#ifndef SAPPHIRE_AST_NODE_STATEMENT_EXPRESSIONSTATEMENT_H_
#define SAPPHIRE_AST_NODE_STATEMENT_EXPRESSIONSTATEMENT_H_
#include "ast/ast.h"
namespace ast {
namespace node {

class ExpressionStatement : public Statement {
  Expression* expression_;

public:
  ExpressionStatement(long lineNumber, Expression* expr)
      : ExpressionStatement(expr) {
    setLine(lineNumber);
  }
  ExpressionStatement(Expression* expr) : expression_(expr) {}
  virtual ~ExpressionStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  Expression* expression() { return expression_; }
};
} // namespace node
} // namespace ast
#endif
