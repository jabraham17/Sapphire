#include "OperatorDefinition.h"

#include "ast/node/NodeList.h"
#include "ast/node/statement/Scope.h"
namespace ast {
namespace node {

ASTNode* OperatorDefinition::clone() {
  return new OperatorDefinition(
      op_,
      toNodeType<std::remove_pointer_t<decltype(parameters_)>>(
          parameters_->clone()),
      toNodeType<std::remove_pointer_t<decltype(body_)>>(body_->clone()));
}

void OperatorDefinition::replaceNode(ASTNode* old, ASTNode* replacement) {
  if(parameters_ == old) {
    replacement->parent() = this;
    parameters_ =
        toNodeType<std::remove_pointer_t<decltype(parameters_)>>(replacement);
    return;
  }
  if(body_ == old) {
    replacement->parent() = this;
    body_ = toNodeType<std::remove_pointer_t<decltype(body_)>>(replacement);
    return;
  }
}

OperatorDefinition::OperatorDefinition(
    long line,
    OperatorType op,
    NodeList* parameters,
    Scope* body)
    : OperatorDefinition(op, parameters, body) {
  setLine(line);
}
OperatorDefinition::OperatorDefinition(
    OperatorType op,
    NodeList* parameters,
    Scope* body)
    : op_(op), parameters_(parameters), body_(body) {
  parameters_->parent() = this;
  body_->parent() = this;
}

OperatorType OperatorDefinition::opType() { return op_; }
NodeList* OperatorDefinition::parameters() { return parameters_; }
Scope* OperatorDefinition::body() { return body_; }

} // namespace node
} // namespace ast
