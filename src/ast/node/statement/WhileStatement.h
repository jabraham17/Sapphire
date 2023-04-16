#ifndef SAPPHIRE_AST_NODE_STATEMENT_WHILESTATEMENT_H_
#define SAPPHIRE_AST_NODE_STATEMENT_WHILESTATEMENT_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {
class WhileStatement : public Statement {
  Expression* expr_;
  Scope* body_;

public:
  WhileStatement(long lineNumber, Expression* expr, Scope* body)
      : WhileStatement(expr, body) {
    setLine(lineNumber);
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
