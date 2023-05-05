#include "WhileStatement.h"

#include "Scope.h"

#include "ast/node/expression/Expression.h"
namespace ast {
namespace node {

// ASTNode* WhileStatement::clone() {
//   return new WhileStatement(
//       toNodeType<std::remove_pointer_t<decltype(expr_)>>(expr_->clone()),
//       toNodeType<std::remove_pointer_t<decltype(body_)>>(body_->clone()));
// }

WhileStatement::WhileStatement::WhileStatement(
    long line,
    Expression* expr,
    Scope* body)
    : WhileStatement(expr, body) {
  setLine(line);
}
WhileStatement::WhileStatement(Expression* expr, Scope* body) {
  this->exprIdx_ = this->addChild(expr);
  this->bodyIdx_ = this->addChild(body);
}

Expression* WhileStatement::expr() {
  return child(this->exprIdx_)->toExpression();
}
Scope* WhileStatement::body() { return child(this->bodyIdx_)->toScope(); }

} // namespace node
} // namespace ast
