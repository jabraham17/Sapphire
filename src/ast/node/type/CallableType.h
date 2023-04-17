#ifndef SAPPHIRE_AST_NODE_TYPE_CALLABLETYPE_H_
#define SAPPHIRE_AST_NODE_TYPE_CALLABLETYPE_H_
#include "Type.h"
#include "TypeList.h"

#include "ast/ast.h"
namespace ast {
namespace node {
// todo: move to cpp definition stuff

class CallableType : public Type {

private:
  TypeList* parameterTypes_;
  Type* returnType_;

protected:
  virtual void replaceNode(ASTNode* old, ASTNode* replacement) override;

public:
  CallableType(long line, TypeList* parameterTypes, Type* returnType);
  CallableType(TypeList* parameterTypes, Type* returnType);
  CallableType(
      long line,
      TypeList* parameterTypes,
      Type* returnType,
      bool isRef,
      bool isNilable);
  CallableType(
      TypeList* parameterTypes,
      Type* returnType,
      bool isRef,
      bool isNilable);
  virtual ~CallableType() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual ASTNode* clone() override;

  TypeList* parameterTypes();
  Type* returnType();
};
} // namespace node
} // namespace ast
#endif
