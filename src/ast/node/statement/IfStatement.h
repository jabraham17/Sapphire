#ifndef SAPPHIRE_AST_NODE_STATEMENT_IFSTATEMENT_H_
#define SAPPHIRE_AST_NODE_STATEMENT_IFSTATEMENT_H_
#include "ast/ast.h"
namespace ast {
namespace node {
class IfStatement : public Statement {
  Expression* expr_;
  Scope* ifBody_;
  // can be nullptr
  Statement* elseBody_; // either Scope or IfStatement
public:
  IfStatement(
      long lineNumber,
      Expression* expr,
      Scope* ifBody,
      Statement* elseBody)
      : IfStatement(expr, ifBody, elseBody) {
    setLine(lineNumber);
  }
  IfStatement(Expression* expr, Scope* ifBody, Statement* elseBody)
      : expr_(expr), ifBody_(ifBody), elseBody_(elseBody) {}
  IfStatement(long lineNumber, Expression* expr, Scope* ifBody)
      : IfStatement(expr, ifBody) {
    setLine(lineNumber);
  }
  IfStatement(Expression* expr, Scope* ifBody)
      : IfStatement(expr, ifBody, nullptr) {}
  virtual ~IfStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  Expression* expr() { return expr_; }
  Scope* ifBody() { return ifBody_; }
  bool hasElseBody();
  bool hasPlainElse();
  bool hasElseIf();
  Scope* plainElseBody();
  IfStatement* elseIfBody();
};
} // namespace node
} // namespace ast
#endif
