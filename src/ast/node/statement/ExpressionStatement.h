#ifndef SAPPHIRE_AST_NODE_STATEMENT_EXPRESSIONSTATEMENT_H_
#define SAPPHIRE_AST_NODE_STATEMENT_EXPRESSIONSTATEMENT_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {

// todo: move to cpp definition stuff

class ExpressionStatement : public Statement {
  Expression* expression_;

public:
  ExpressionStatement(long line, Expression* expr) : ExpressionStatement(expr) {
    setLine(line);
  }
  ExpressionStatement(Expression* expr) : expression_(expr) {}
  virtual ~ExpressionStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  Expression* expression() { return expression_; }
};
} // namespace node
} // namespace ast
#endif
