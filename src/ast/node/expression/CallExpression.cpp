#include "CallExpression.h"

#include <cassert>
namespace ast {
namespace node {

ASTNode* CallExpression::clone() {
  assert(false && "call expr clone is unimplemented");
  return nullptr;
}

void CallExpression::replaceNode(ASTNode* old, ASTNode* replacement) {
  if(operands_ == old) {
    replacement->parent() = this;
    operands_ =
        toNodeType<std::remove_pointer_t<decltype(operands_)>>(replacement);
    return;
  }
  if(type_ == old) {
    replacement->parent() = this;
    type_ = toNodeType<std::remove_pointer_t<decltype(type_)>>(replacement);
    return;
  }
}

Type* CallExpression::type() { return this->type_; }
void CallExpression::setType(Type* type) {
  this->type_ = type;
  this->type_->parent() = this;
}

OperatorType CallExpression::opType() { return this->op_; }
NodeList* CallExpression::operands() { return this->operands_; }

} // namespace node
} // namespace ast
