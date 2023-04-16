#ifndef SAPPHIRE_AST_NODE_TYPE_PRIMITIVETYPE_H_
#define SAPPHIRE_AST_NODE_TYPE_PRIMITIVETYPE_H_
#include "Type.h"

#include "ast/ast.h"

namespace ast {
namespace node {

class PrimitiveType : public Type {

private:
  PrimitiveTypeEnum primType;

public:
  PrimitiveType(long lineNumber, PrimitiveTypeEnum type) : PrimitiveType(type) {
    setLine(lineNumber);
  }
  PrimitiveType(PrimitiveTypeEnum type) : PrimitiveType(type, false, false) {}
  PrimitiveType(
      long lineNumber,
      PrimitiveTypeEnum type,
      bool isRef,
      bool isNilable)
      : PrimitiveType(type, isRef, isNilable) {
    setLine(lineNumber);
  }
  PrimitiveType(PrimitiveTypeEnum type, bool isRef, bool isNilable)
      : Type(isRef, isNilable), primType(type) {}
  virtual ~PrimitiveType() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  PrimitiveTypeEnum primitiveType() { return primType; }
};
} // namespace node
} // namespace ast
#endif
