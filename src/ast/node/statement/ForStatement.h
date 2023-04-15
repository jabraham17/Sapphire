#ifndef SAPPHIRE_AST_NODE_FORSTATEMENT_H_
#define SAPPHIRE_AST_NODE_FORSTATEMENT_H_
#include "ast/ast.h"
namespace ast {
namespace node {

class ForStatement : public Statement {
  DefExpression* var_;
  Expression* expr_;
  Scope* body_;

public:
  ForStatement(
      long lineNumber,
      DefExpression* var,
      Expression* expr,
      Scope* body)
      : ForStatement(var, expr, body) {
    setLine(lineNumber);
  }
  ForStatement(DefExpression* var, Expression* expr, Scope* body)
      : var_(var), expr_(expr), body_(body) {}
  virtual ~ForStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  DefExpression* variable() { return var_; }
  Expression* expr() { return expr_; }
  Scope* body() { return body_; }
};
} // namespace node
} // namespace ast
#endif
