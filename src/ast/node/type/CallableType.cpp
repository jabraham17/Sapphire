#include "CallableType.h"
// #include "Type.h"
// #include <cassert>

namespace ast {
namespace node {

ASTNode* CallableType::clone() {
  auto params_ = this->parameterTypes();
  ASTList params;
  params.reserve(params_.size());
  for(const Type* p : params_)
    params.push_back((ASTNode*)p);
  // ASTList params(params_.begin(), params_.end());

  return new CallableType(
      params,
      this->returnType()->clone()->toType(),
      this->isRef(),
      this->isNilable());
}

CallableType::CallableType(
    long line,
    const ASTList& parameterTypes,
    Type* returnType)
    : CallableType(parameterTypes, returnType) {
  setLine(line);
}
CallableType::CallableType(const ASTList& parameterTypes, Type* returnType)
    : CallableType(parameterTypes, returnType, false, false) {}
CallableType::CallableType(
    long line,
    const ASTList& parameterTypes,
    Type* returnType,
    bool isRef,
    bool isNilable)
    : CallableType(parameterTypes, returnType, isRef, isNilable) {
  setLine(line);
}
CallableType::CallableType(
    const ASTList& parameterTypes,
    Type* returnType,
    bool isRef,
    bool isNilable)
    : Type(isRef, isNilable) {
  this->returnTypeIdx_ = addChild(returnType);
  this->parameterTypesStartIdx_ = numChildren();
  for(auto p : parameterTypes) {
    addChild(p);
  }
  this->parameterTypesStopIdx_ =
      this->parameterTypesStartIdx_ + parameterTypes.size();
}

ASTListIteratorPair<Type> CallableType::parameterTypes() {
  return children_slice<Type>(
      this->parameterTypesStartIdx_,
      this->parameterTypesStopIdx_);
}
Type* CallableType::returnType() {
  return child(this->returnTypeIdx_)->toType();
}

} // namespace node
} // namespace ast
