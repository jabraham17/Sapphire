#include "ExpressionStatement.h"

#include "ast/node/expression/Expression.h"
namespace ast {
namespace node {

// ASTNode* ExpressionStatement::clone() {
//   return new ExpressionStatement(
//       toNodeType<std::remove_pointer_t<decltype(expression_)>>(
//           expression_->clone()));
// }

ExpressionStatement::ExpressionStatement(long line, Expression* expr)
    : ExpressionStatement(expr) {
  setLine(line);
}
ExpressionStatement::ExpressionStatement(Expression* expr) {
  this->expressionIdx_ = this->addChild(expr);
}

Expression* ExpressionStatement::expression() {
  return child(this->expressionIdx_)->toExpression();
}

} // namespace node
} // namespace ast
