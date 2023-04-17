#include "ReturnStatement.h"

#include "ast/node/expression/Expression.h"
namespace ast {
namespace node {
void ReturnStatement::replaceNode(ASTNode* old, ASTNode* replacement) {
  if(expr_ == old) {
    replacement->parent() = this;
    expr_ = toNodeType<std::remove_pointer_t<decltype(expr_)>>(replacement);
    return;
  }
}
ReturnStatement::ReturnStatement(long line, Expression* expr)
    : ReturnStatement(expr) {
  setLine(line);
}
ReturnStatement::ReturnStatement(Expression* expr) : expr_(expr) {
  expr_->parent() = this;
}
Expression* ReturnStatement::expression() { return expr_; }
} // namespace node
} // namespace ast
