#ifndef SAPPHIRE_AST_NODE_STATEMENT_RETURNSTATEMENT_H_
#define SAPPHIRE_AST_NODE_STATEMENT_RETURNSTATEMENT_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {
// todo: move to cpp definition stuff

class ReturnStatement : public Statement {
  Expression* expr_;

public:
  ReturnStatement(long line, Expression* expr) : ReturnStatement(expr) {
    setLine(line);
  }
  ReturnStatement(Expression* expr) : expr_(expr) {}
  virtual ~ReturnStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  Expression* expression() { return expr_; }
};
} // namespace node
} // namespace ast
#endif
