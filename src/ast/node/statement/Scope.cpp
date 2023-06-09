#include "Scope.h"

namespace ast {
namespace node {

// ASTNode* Scope::clone() {
//   return new Scope(toNodeType<std::remove_pointer_t<decltype(statements_)>>(
//       statements_->clone()));
// }

Scope::Scope(long line, const ASTList& statements) : Scope(statements) {
  setLine(line);
};
Scope::Scope(const ASTList& statements) {
  this->statementsStartIdx_ = this->numChildren();
  for(auto p : statements) {
    this->addChild(p);
  }
  this->numStatements_ = statements.size();
};
Scope::Scope(long line) : Scope() { setLine(line); }
Scope::Scope() {
  this->statementsStartIdx_ = this->numChildren();
  this->numStatements_ = 0;
}

ASTListIteratorPair<Statement> Scope::statements() {
  return children_slice<Statement>(
      this->statementsStartIdx_,
      this->numStatements_);
}

} // namespace node
} // namespace ast
