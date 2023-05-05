#include "ForStatement.h"

#include "Scope.h"

#include "ast/node/expression/DefExpression.h"
#include "ast/node/expression/Expression.h"

namespace ast {
namespace node {

// ASTNode* ForStatement::clone() {
//   return new ForStatement(
//       toNodeType<std::remove_pointer_t<decltype(var_)>>(var_->clone()),
//       toNodeType<std::remove_pointer_t<decltype(expr_)>>(expr_->clone()),
//       toNodeType<std::remove_pointer_t<decltype(body_)>>(body_->clone()));
// }

ForStatement::ForStatement(
    long line,
    DefExpression* var,
    Expression* expr,
    Scope* body)
    : ForStatement(var, expr, body) {
  setLine(line);
}
ForStatement::ForStatement(DefExpression* var, Expression* expr, Scope* body) {
  this->varIdx_ = this->addChild(var);
  this->exprIdx_ = this->addChild(expr);
  this->bodyIdx_ = this->addChild(body);
}

DefExpression* ForStatement::variable() {
  return child(this->varIdx_)->toDefExpression();
}
Expression* ForStatement::expr() {
  return child(this->exprIdx_)->toExpression();
}
Scope* ForStatement::body() { return child(this->bodyIdx_)->toScope(); }

} // namespace node
} // namespace ast
