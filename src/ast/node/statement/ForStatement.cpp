#include "ForStatement.h"

#include "Scope.h"

#include "ast/node/expression/DefExpression.h"
#include "ast/node/expression/Expression.h"

namespace ast {
namespace node {

ASTNode* ForStatement::clone() {
  return new ForStatement(
      toNodeType<std::remove_pointer_t<decltype(var_)>>(var_->clone()),
      toNodeType<std::remove_pointer_t<decltype(expr_)>>(expr_->clone()),
      toNodeType<std::remove_pointer_t<decltype(body_)>>(body_->clone()));
}

void ForStatement::replaceNode(ASTNode* old, ASTNode* replacement) {
  if(var_ == old) {
    replacement->parent() = this;
    var_ = toNodeType<std::remove_pointer_t<decltype(var_)>>(replacement);
    return;
  }
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

ForStatement::ForStatement(
    long line,
    DefExpression* var,
    Expression* expr,
    Scope* body)
    : ForStatement(var, expr, body) {
  setLine(line);
}
ForStatement::ForStatement(DefExpression* var, Expression* expr, Scope* body)
    : var_(var), expr_(expr), body_(body) {
  var_->parent() = this;
  expr_->parent() = this;
  body_->parent() = this;
}

DefExpression* ForStatement::variable() { return var_; }
Expression* ForStatement::expr() { return expr_; }
Scope* ForStatement::body() { return body_; }

} // namespace node
} // namespace ast
