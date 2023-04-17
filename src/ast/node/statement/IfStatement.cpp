#include "IfStatement.h"

#include "Scope.h"

#include "ast/node/expression/Expression.h"
namespace ast {
namespace node {

void IfStatement::replaceNode(ASTNode* old, ASTNode* replacement) {
  if(expr_ == old) {
    replacement->parent() = this;
    expr_ = toNodeType<std::remove_pointer_t<decltype(expr_)>>(replacement);
    return;
  }
  if(ifBody_ == old) {
    replacement->parent() = this;
    ifBody_ = toNodeType<std::remove_pointer_t<decltype(ifBody_)>>(replacement);
    return;
  }
  if(elseBody_ == old) {
    replacement->parent() = this;
    elseBody_ =
        toNodeType<std::remove_pointer_t<decltype(elseBody_)>>(replacement);
    return;
  }
}

IfStatement::IfStatement(
    long line,
    Expression* expr,
    Scope* ifBody,
    Statement* elseBody)
    : IfStatement(expr, ifBody, elseBody) {
  setLine(line);
}
IfStatement::IfStatement(long line, Expression* expr, Scope* ifBody)
    : IfStatement(expr, ifBody) {
  setLine(line);
}
IfStatement::IfStatement(Expression* expr, Scope* ifBody)
    : IfStatement(expr, ifBody, nullptr) {}
IfStatement::IfStatement(Expression* expr, Scope* ifBody, Statement* elseBody)
    : expr_(expr), ifBody_(ifBody), elseBody_(elseBody) {
  expr_->parent() = this;
  ifBody_->parent() = this;
  if(elseBody_) elseBody_->parent() = this;
}

Expression* IfStatement::expr() { return expr_; }
Scope* IfStatement::ifBody() { return ifBody_; }

bool IfStatement::hasElseBody() { return elseBody_ != nullptr; }
bool IfStatement::hasPlainElse() {
  return hasElseBody() && isScopeNode(elseBody_);
}
bool IfStatement::hasElseIf() {
  return hasElseBody() && isIfStatementNode(elseBody_);
}
Scope* IfStatement::plainElseBody() { return toScopeNode(elseBody_); }
IfStatement* IfStatement::elseIfBody() { return toIfStatementNode(elseBody_); }

} // namespace node
} // namespace ast
