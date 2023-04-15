#ifndef SAPPHIRE_AST_NODE_TYPE_ARRAYTYPE_H_
#define SAPPHIRE_AST_NODE_TYPE_ARRAYTYPE_H_
#include "Type.h"

#include "ast/ast.h"
namespace ast {
namespace node {
class ArrayType : public Type {

private:
  Type* elementType_;

public:
  ArrayType(long lineNumber, Type* elementType) : ArrayType(elementType) {
    setLine(lineNumber);
  }
  ArrayType(Type* elementType) : ArrayType(elementType, false, false) {}
  ArrayType(long lineNumber, Type* elementType, bool isRef, bool isNilable)
      : ArrayType(elementType, isRef, isNilable) {
    setLine(lineNumber);
  }
  ArrayType(Type* elementType, bool isRef, bool isNilable)
      : Type(isRef, isNilable), elementType_(elementType) {}
  virtual ~ArrayType() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  Type* elementType() { return elementType_; }
};

} // namespace node
} // namespace ast
#endif
