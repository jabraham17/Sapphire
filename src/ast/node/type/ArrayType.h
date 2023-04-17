#ifndef SAPPHIRE_AST_NODE_TYPE_ARRAYTYPE_H_
#define SAPPHIRE_AST_NODE_TYPE_ARRAYTYPE_H_
#include "Type.h"

#include "ast/ast.h"
namespace ast {
namespace node {
// todo: move to cpp definition stuff

class ArrayType : public Type {
private:
  Type* elementType_;

protected:
  virtual void replaceNode(ASTNode* old, ASTNode* replacement) override;

public:
  ArrayType(long line, Type* elementType);
  ArrayType(Type* elementType);
  ArrayType(long line, Type* elementType, bool isRef, bool isNilable)
      : ArrayType(elementType, isRef, isNilable) {
    setLine(line);
  }
  ArrayType(Type* elementType, bool isRef, bool isNilable);
  virtual ~ArrayType() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  Type* elementType();
};

} // namespace node
} // namespace ast
#endif
