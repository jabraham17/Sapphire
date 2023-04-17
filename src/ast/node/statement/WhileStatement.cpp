#include "WhileStatement.h"

#include "Scope.h"

#include "ast/node/expression/Expression.h"
namespace ast {
namespace node {

void WhileStatement::replaceNode(ASTNode* old, ASTNode* replacement) {
  if(expr_ == old) {
    replacement->parent() = this;
    expr_ = toNodeType<std::remove_pointer_t<decltype(expr_)>>(replacement);
    return;
  }
  if(body_ == old) {
    replacement->parent() = this;
    body_ = toNodeType<std::remove_pointer_t<decltype(body_)>>(replacement);
    return;
  }
}

WhileStatement::WhileStatement::WhileStatement(
    long line,
    Expression* expr,
    Scope* body)
    : WhileStatement(expr, body) {
  setLine(line);
}
WhileStatement::WhileStatement(Expression* expr, Scope* body)
    : expr_(expr), body_(body) {
  expr_->parent() = this;
  body_->parent() = this;
}

Expression* WhileStatement::expr() { return expr_; }
Scope* WhileStatement::body() { return body_; }

} // namespace node
} // namespace ast
