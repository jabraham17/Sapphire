#include "CallExpression.h"

#include <cassert>
namespace ast {
namespace node {

// ASTNode* CallExpression::clone() {
//   assert(false && "call expr clone is unimplemented");
//   return nullptr;
// }

Type* CallExpression::type() { return child(this->typeIdx_)->toType(); }
void CallExpression::setType(Type* type) {
  // auto old = this->child(this->typeIdx_);
  this->setChild(this->typeIdx_, type);
  // delete old;
}

void CallExpression::addOperand(ASTNode* node) {
  this->addChild(node);
  this->numOperands++;
}
void CallExpression::addOperands(const ASTList& nodes) {
  for(auto n : nodes) {
    this->addOperand(n);
  }
}

OperatorType CallExpression::opType() { return this->op_; }
ASTListIteratorPair<ASTNode> CallExpression::operands() {
  return children_slice(this->operandsStartIdx_, this->numOperands);
}

} // namespace node
} // namespace ast
