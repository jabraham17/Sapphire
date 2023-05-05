#include "ExternDefinition.h"

#include "FunctionPrototype.h"
namespace ast {
namespace node {

// ASTNode* ExternDefinition::clone() {
//   return new ExternDefinition(
//       toNodeType<std::remove_pointer_t<decltype(functionPrototype_)>>(
//           functionPrototype_->clone()));
// }

ExternDefinition::ExternDefinition(
    long line,
    FunctionPrototype* functionPrototype)
    : ExternDefinition(functionPrototype) {
  setLine(line);
}
ExternDefinition::ExternDefinition(FunctionPrototype* functionPrototype) {
  this->functionPrototypeIdx_ = this->addChild(functionPrototype);
}

FunctionPrototype* ExternDefinition::functionPrototype() {
  return child(this->functionPrototypeIdx_)->toFunctionPrototype();
}
} // namespace node
} // namespace ast
