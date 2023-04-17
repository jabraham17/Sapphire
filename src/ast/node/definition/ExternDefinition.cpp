#include "ExternDefinition.h"

#include "FunctionPrototype.h"
namespace ast {
namespace node {

void ExternDefinition::replaceNode(ASTNode* old, ASTNode* replacement) {
  if(functionPrototype_ == old) {
    replacement->parent() = this;
    functionPrototype_ =
        toNodeType<std::remove_pointer_t<decltype(functionPrototype_)>>(
            replacement);
    return;
  }
}

ExternDefinition::ExternDefinition(
    long line,
    FunctionPrototype* functionPrototype)
    : ExternDefinition(functionPrototype) {
  setLine(line);
}
ExternDefinition::ExternDefinition(FunctionPrototype* functionPrototype)
    : functionPrototype_(functionPrototype) {
  functionPrototype_->parent() = this;
}

FunctionPrototype* ExternDefinition::functionPrototype() {
  return functionPrototype_;
}
} // namespace node
} // namespace ast
