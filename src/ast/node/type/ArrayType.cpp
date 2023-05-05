#include "ArrayType.h"
namespace ast {
namespace node {

ASTNode* ArrayType::clone() {
  auto xElmType = this->elementType()->clone()->toType();
  auto x = new ArrayType(xElmType, this->isRef(), this->isNilable());
  return x;
}

ArrayType::ArrayType(long line, Type* elementType) : ArrayType(elementType) {
  setLine(line);
}
ArrayType::ArrayType(Type* elementType)
    : ArrayType(elementType, false, false) {}
ArrayType::ArrayType(Type* elementType, bool isRef, bool isNilable)
    : Type(isRef, isNilable) {
  this->elementTypeIdx_ = this->addChild(elementType);
}

Type* ArrayType::elementType() {
  return child(this->elementTypeIdx_)->toType();
}

} // namespace node
} // namespace ast
