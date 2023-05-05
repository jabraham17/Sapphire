#include "FunctionDefinition.h"

#include "FunctionPrototype.h"

#include "ast/node/statement/Scope.h"
namespace ast {
namespace node {

// ASTNode* FunctionDefinition::clone() {
//   return new FunctionDefinition(
//       toNodeType<std::remove_pointer_t<decltype(functionPrototype_)>>(
//           functionPrototype_->clone()),
//       toNodeType<std::remove_pointer_t<decltype(body_)>>(body_->clone()));
// }

FunctionDefinition::FunctionDefinition(
    long line,
    FunctionPrototype* functionPrototype,
    Scope* body)
    : FunctionDefinition(functionPrototype, body) {
  setLine(line);
}
FunctionDefinition::FunctionDefinition(
    FunctionPrototype* functionPrototype,
    Scope* body) {
  this->functionPrototypeIdx_ = this->addChild(functionPrototype);
  this->bodyIdx_ = this->addChild(body);
}

FunctionPrototype* FunctionDefinition::functionPrototype() {
  return child(this->functionPrototypeIdx_)->toFunctionPrototype();
}
Scope* FunctionDefinition::body() { return child(this->bodyIdx_)->toScope(); }
} // namespace node
} // namespace ast
