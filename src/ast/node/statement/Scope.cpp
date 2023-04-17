#include "Scope.h"

#include "ast/node/NodeList.h"
namespace ast {
namespace node {

ASTNode* Scope::clone() {
  return new Scope(toNodeType<std::remove_pointer_t<decltype(statements_)>>(
      statements_->clone()));
}

void Scope::replaceNode(ASTNode* old, ASTNode* replacement) {
  if(statements_ == old) {
    replacement->parent() = this;
    statements_ =
        toNodeType<std::remove_pointer_t<decltype(statements_)>>(replacement);
    return;
  }
}
Scope::Scope(long line, NodeList* statements) : Scope(statements) {
  setLine(line);
};
Scope::Scope(NodeList* statements) : statements_(statements) {
  statements_->parent() = this;
};

NodeList* Scope::statements() { return statements_; }

} // namespace node
} // namespace ast
