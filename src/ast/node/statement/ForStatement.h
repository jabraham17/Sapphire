#ifndef SAPPHIRE_AST_NODE_STATEMENT_FORSTATEMENT_H_
#define SAPPHIRE_AST_NODE_STATEMENT_FORSTATEMENT_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {
// todo: move to cpp definition stuff

class ForStatement : public Statement {
  DefExpression* var_;
  Expression* expr_;
  Scope* body_;

public:
  ForStatement(long line, DefExpression* var, Expression* expr, Scope* body)
      : ForStatement(var, expr, body) {
    setLine(line);
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
