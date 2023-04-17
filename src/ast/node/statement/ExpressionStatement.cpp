#include "ExpressionStatement.h"

#include "ast/node/expression/Expression.h"
namespace ast {
namespace node {

void ExpressionStatement::replaceNode(ASTNode* old, ASTNode* replacement) {
  if(expression_ == old) {
    replacement->parent() = this;
    expression_ =
        toNodeType<std::remove_pointer_t<decltype(expression_)>>(replacement);
    return;
  }
}
ExpressionStatement::ExpressionStatement(long line, Expression* expr)
    : ExpressionStatement(expr) {
  setLine(line);
}
ExpressionStatement::ExpressionStatement(Expression* expr) : expression_(expr) {
  expression_->parent() = this;
}

Expression* ExpressionStatement::expression() { return expression_; }

} // namespace node
} // namespace ast
