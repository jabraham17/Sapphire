#ifndef SAPPHIRE_AST_NODE_STATEMENT_IFSTATEMENT_H_
#define SAPPHIRE_AST_NODE_STATEMENT_IFSTATEMENT_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {
// todo: move to cpp definition stuff

class IfStatement : public Statement {
  std::size_t exprIdx_;
  std::size_t ifBodyIdx_;
  std::ptrdiff_t elseBodyIdx_; // either Scope or IfStatement

public:
  IfStatement(long line, Expression* expr, Scope* ifBody, Statement* elseBody);
  IfStatement(Expression* expr, Scope* ifBody, Statement* elseBody);
  IfStatement(long line, Expression* expr, Scope* ifBody);
  IfStatement(Expression* expr, Scope* ifBody);
  virtual ~IfStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  // virtual ASTNode* clone() override;

  CONST_MEMBER_FUNC(Expression*, expr)
  CONST_MEMBER_FUNC(Scope*, ifBody)

  CONST_MEMBER_FUNC(bool, hasElseBody)
  CONST_MEMBER_FUNC(Statement*, elseBody)
  CONST_MEMBER_FUNC(bool, hasPlainElse)
  CONST_MEMBER_FUNC(Scope*, plainElseBody)
  CONST_MEMBER_FUNC(bool, hasElseIf)
  CONST_MEMBER_FUNC(IfStatement*, elseIfBody)
};
} // namespace node
} // namespace ast
#endif
