#include "InitDefinition.h"

#include "ast/node/statement/Scope.h"
namespace ast {
namespace node {

// ASTNode* InitDefinition::clone() {
//   return new InitDefinition(
//       toNodeType<std::remove_pointer_t<decltype(parameters_)>>(
//           parameters_->clone()),
//       toNodeType<std::remove_pointer_t<decltype(body_)>>(body_->clone()));
// }

InitDefinition::InitDefinition(
    long line,
    const ASTList& parameters,
    Scope* body)
    : InitDefinition(parameters, body) {
  setLine(line);
}
InitDefinition::InitDefinition(long line, Scope* body) : InitDefinition(body) {
  setLine(line);
}
InitDefinition::InitDefinition(const ASTList& parameters, Scope* body)
    : isInit_(true) {
  this->bodyIdx_ = this->addChild(body);
  this->parametersStartIdx_ = this->numChildren();
  for(auto p : parameters) {
    this->addChild(p);
  }
  this->numParameters_ = parameters.size();
}
InitDefinition::InitDefinition(Scope* body) : isInit_(false) {
  this->bodyIdx_ = this->addChild(body);
  this->parametersStartIdx_ = -1;
  this->numParameters_ = 0;
}

bool InitDefinition::isDeinit() { return !isInit_; }
bool InitDefinition::isInit() { return isInit_; }
ASTListIteratorPair<Parameter> InitDefinition::parameters() {
  return children_slice<Parameter>(
      this->parametersStartIdx_,
      this->numParameters_);
}
Scope* InitDefinition::body() { return child(this->bodyIdx_)->toScope(); }
} // namespace node
} // namespace ast
