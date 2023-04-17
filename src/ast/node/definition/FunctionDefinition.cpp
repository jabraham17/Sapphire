#include "FunctionDefinition.h"

#include "FunctionPrototype.h"

#include "ast/node/statement/Scope.h"
namespace ast {
namespace node {

void FunctionDefinition::replaceNode(ASTNode* old, ASTNode* replacement) {
  if(functionPrototype_ == old) {
    replacement->parent() = this;
    functionPrototype_ =
        toNodeType<std::remove_pointer_t<decltype(functionPrototype_)>>(
            replacement);
    return;
  }
  if(body_ == old) {
    replacement->parent() = this;
    body_ = toNodeType<std::remove_pointer_t<decltype(body_)>>(replacement);
    return;
  }
}

FunctionDefinition::FunctionDefinition(
    long line,
    FunctionPrototype* functionPrototype,
    Scope* body)
    : FunctionDefinition(functionPrototype, body) {
  setLine(line);
}
FunctionDefinition::FunctionDefinition(
    FunctionPrototype* functionPrototype,
    Scope* body)
    : functionPrototype_(functionPrototype), body_(body) {
  functionPrototype_->parent() = this;
  body->parent() = this;
}

FunctionPrototype* FunctionDefinition::functionPrototype() {
  return functionPrototype_;
}
Scope* FunctionDefinition::body() { return body_; }
} // namespace node
} // namespace ast
