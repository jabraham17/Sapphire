#include "CallableType.h"
// #include "TypeList.h"
// #include "ast/node/NodeList.h"
// #include "Type.h"
// #include <cassert>

namespace ast {
namespace node {

ASTNode* CallableType::clone() {
  return new CallableType(
      toNodeType<std::remove_pointer_t<decltype(parameterTypes_)>>(
          parameterTypes_->clone()),
      toNodeType<std::remove_pointer_t<decltype(returnType_)>>(
          returnType_->clone()),
      this->isRef(),
      this->isNilable());
}

void CallableType::replaceNode(ASTNode* old, ASTNode* replacement) {
  if(parameterTypes_ == old) {
    replacement->parent() = this;
    parameterTypes_ =
        toNodeType<std::remove_pointer_t<decltype(parameterTypes_)>>(
            replacement);
    return;
  }
  if(returnType_ == old) {
    replacement->parent() = this;
    returnType_ =
        toNodeType<std::remove_pointer_t<decltype(returnType_)>>(replacement);
    return;
  }
}

CallableType::CallableType(
    long line,
    TypeList* parameterTypes,
    Type* returnType)
    : CallableType(parameterTypes, returnType) {
  setLine(line);
}
CallableType::CallableType(TypeList* parameterTypes, Type* returnType)
    : CallableType(parameterTypes, returnType, false, false) {}
CallableType::CallableType(
    long line,
    TypeList* parameterTypes,
    Type* returnType,
    bool isRef,
    bool isNilable)
    : CallableType(parameterTypes, returnType, isRef, isNilable) {
  setLine(line);
}
CallableType::CallableType(
    TypeList* parameterTypes,
    Type* returnType,
    bool isRef,
    bool isNilable)
    : Type(isRef, isNilable), parameterTypes_(parameterTypes),
      returnType_(returnType) {
  parameterTypes_->parent() = this;
  returnType_->parent() = this;
}

TypeList* CallableType::parameterTypes() { return this->parameterTypes_; }
Type* CallableType::returnType() { return this->returnType_; }

} // namespace node
} // namespace ast
