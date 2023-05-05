#include "ReturnStatement.h"

#include "ast/node/expression/Expression.h"
namespace ast {
namespace node {

// ASTNode* ReturnStatement::clone() {
//   return new ReturnStatement(
//       toNodeType<std::remove_pointer_t<decltype(expr_)>>(expr_->clone()));
// }

ReturnStatement::ReturnStatement(long line, Expression* expr)
    : ReturnStatement(expr) {
  setLine(line);
}
ReturnStatement::ReturnStatement(Expression* expr) {
  this->exprIdx_ = this->addChild(expr);
}
Expression* ReturnStatement::expression() {
  return child(this->exprIdx_)->toExpression();
}
} // namespace node
} // namespace ast
