#include "IfStatement.h"
namespace ast {
namespace node {

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
