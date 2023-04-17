#ifndef SAPPHIRE_AST_NODE_STATEMENT_IFSTATEMENT_H_
#define SAPPHIRE_AST_NODE_STATEMENT_IFSTATEMENT_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {
// todo: move to cpp definition stuff

class IfStatement : public Statement {
  Expression* expr_;
  Scope* ifBody_;
  // can be nullptr
  Statement* elseBody_; // either Scope or IfStatement

protected:
  virtual void replaceNode(ASTNode* old, ASTNode* replacement) override;

public:
  IfStatement(long line, Expression* expr, Scope* ifBody, Statement* elseBody);
  IfStatement(Expression* expr, Scope* ifBody, Statement* elseBody);
  IfStatement(long line, Expression* expr, Scope* ifBody);
  IfStatement(Expression* expr, Scope* ifBody);
  virtual ~IfStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  Expression* expr();
  Scope* ifBody();

  bool hasElseBody();
  bool hasPlainElse();
  bool hasElseIf();
  Scope* plainElseBody();
  IfStatement* elseIfBody();
};
} // namespace node
} // namespace ast
#endif
