#include "IfStatement.h"

#include "Scope.h"

#include "ast/node/expression/Expression.h"

#include <cassert>
namespace ast {
namespace node {

// ASTNode* IfStatement::clone() {
//   assert(false && "if statement clone is unimplemented");
//   return nullptr;
// }

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
IfStatement::IfStatement(Expression* expr, Scope* ifBody, Statement* elseBody) {
  this->exprIdx_ = this->addChild(expr);
  this->ifBodyIdx_ = this->addChild(ifBody);
  if(elseBody) this->elseBodyIdx_ = this->addChild(elseBody);
  else this->elseBodyIdx_ = -1;
}

Expression* IfStatement::expr() {
  return child(this->exprIdx_)->toExpression();
}
Scope* IfStatement::ifBody() { return child(this->ifBodyIdx_)->toScope(); }
Statement* IfStatement::elseBody() {
  if(hasElseBody()) return child(this->elseBodyIdx_)->toStatement();
  else return nullptr;
}

bool IfStatement::hasElseBody() { return this->elseBodyIdx_ != -1; }
bool IfStatement::hasPlainElse() {
  return hasElseBody() && elseBody()->isScope();
}
bool IfStatement::hasElseIf() {
  return hasElseBody() && elseBody()->isIfStatement();
}
Scope* IfStatement::plainElseBody() { return elseBody()->toScope(); }
IfStatement* IfStatement::elseIfBody() { return elseBody()->toIfStatement(); }

} // namespace node
} // namespace ast
