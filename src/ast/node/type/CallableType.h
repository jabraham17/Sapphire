#ifndef SAPPHIRE_AST_NODE_TYPE_CALLABLETYPE_H_
#define SAPPHIRE_AST_NODE_TYPE_CALLABLETYPE_H_
#include "Type.h"
#include "TypeList.h"

#include "ast/ast.h"
namespace ast {
namespace node {
class CallableType : public Type {

private:
  TypeList* parameterTypes_;
  Type* returnType_;

public:
  CallableType(long lineNumber, TypeList* parameterTypes, Type* returnType)
      : CallableType(parameterTypes, returnType) {
    setLine(lineNumber);
  }
  CallableType(TypeList* parameterTypes, Type* returnType)
      : CallableType(parameterTypes, returnType, false, false) {}
  CallableType(
      long lineNumber,
      TypeList* parameterTypes,
      Type* returnType,
      bool isRef,
      bool isNilable)
      : CallableType(parameterTypes, returnType, isRef, isNilable) {
    setLine(lineNumber);
  }
  CallableType(
      TypeList* parameterTypes,
      Type* returnType,
      bool isRef,
      bool isNilable)
      : Type(isRef, isNilable), parameterTypes_(parameterTypes),
        returnType_(returnType) {}
  virtual ~CallableType() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  TypeList* parameterTypes() { return parameterTypes_; }
  Type* returnType() { return returnType_; }
};
} // namespace node
} // namespace ast
#endif
