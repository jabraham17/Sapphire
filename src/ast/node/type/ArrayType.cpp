#include "ArrayType.h"
namespace ast {
namespace node {

ASTNode* ArrayType::clone() {
  auto xElmType = toNodeType<std::remove_pointer_t<decltype(elementType_)>>(
      elementType_->clone());
  auto x = new ArrayType(xElmType, this->isRef(), this->isNilable());
  return x;
}

void ArrayType::replaceNode(ASTNode* old, ASTNode* replacement) {
  if(elementType_ == old) {
    replacement->parent() = this;
    elementType_ =
        toNodeType<std::remove_pointer_t<decltype(elementType_)>>(replacement);
    return;
  }
}

ArrayType::ArrayType(long line, Type* elementType) : ArrayType(elementType) {
  setLine(line);
}
ArrayType::ArrayType(Type* elementType)
    : ArrayType(elementType, false, false) {}
ArrayType::ArrayType(Type* elementType, bool isRef, bool isNilable)
    : Type(isRef, isNilable), elementType_(elementType) {}

Type* ArrayType::elementType() { return this->elementType_; }

} // namespace node
} // namespace ast
