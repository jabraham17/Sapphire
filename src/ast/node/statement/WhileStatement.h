#ifndef SAPPHIRE_AST_NODE_STATEMENT_WHILESTATEMENT_H_
#define SAPPHIRE_AST_NODE_STATEMENT_WHILESTATEMENT_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {

// todo: move to cpp definition stuff

class WhileStatement : public Statement {
  Expression* expr_;
  Scope* body_;

public:
  WhileStatement(long line, Expression* expr, Scope* body)
      : WhileStatement(expr, body) {
    setLine(line);
  }
  WhileStatement(Expression* expr, Scope* body) : expr_(expr), body_(body) {}
  virtual ~WhileStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  Expression* expr() { return expr_; }
  Scope* body() { return body_; }
};
} // namespace node
} // namespace ast
#endif
