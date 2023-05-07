#include "OperatorDefinition.h"

#include "ast/node/statement/Scope.h"
namespace ast {
namespace node {

// ASTNode* OperatorDefinition::clone() {
//   return new OperatorDefinition(
//       op_,
//       toNodeType<std::remove_pointer_t<decltype(parameters_)>>(
//           parameters_->clone()),
//       toNodeType<std::remove_pointer_t<decltype(body_)>>(body_->clone()));
// }

OperatorDefinition::OperatorDefinition(
    long line,
    OperatorType op,
    const ASTList& parameters,
    Scope* body)
    : OperatorDefinition(op, parameters, body) {
  setLine(line);
}
OperatorDefinition::OperatorDefinition(
    OperatorType op,
    const ASTList& parameters,
    Scope* body)
    : op_(op) {
  this->bodyIdx_ = this->addChild(body);
  this->parametersStartIdx_ = this->numChildren();
  for(auto p : parameters) {
    this->addChild(p);
  }
  this->numParameters_ = parameters.size();
}

OperatorType OperatorDefinition::opType() { return op_; }
ASTListIteratorPair<Parameter> OperatorDefinition::parameters() {
  return children_slice<Parameter>(
      this->parametersStartIdx_,
      this->numParameters_);
}
Scope* OperatorDefinition::body() { return child(this->bodyIdx_)->toScope(); }

} // namespace node
} // namespace ast
