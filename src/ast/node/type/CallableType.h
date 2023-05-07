#ifndef SAPPHIRE_AST_NODE_TYPE_CALLABLETYPE_H_
#define SAPPHIRE_AST_NODE_TYPE_CALLABLETYPE_H_
#include "Type.h"

#include "ast/ast.h"
namespace ast {
namespace node {
// todo: move to cpp definition stuff

class CallableType : public Type {

private:
  std::size_t returnTypeIdx_;
  std::size_t parameterTypesStartIdx_;
  std::size_t numParameterTypes_;

public:
  CallableType(long line, const ASTList& parameterTypes, Type* returnType);
  CallableType(const ASTList& parameterTypes, Type* returnType);
  CallableType(
      long line,
      const ASTList& parameterTypes,
      Type* returnType,
      bool isRef,
      bool isNilable);
  CallableType(
      const ASTList& parameterTypes,
      Type* returnType,
      bool isRef,
      bool isNilable);
  virtual ~CallableType() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual ASTNode* clone() override;

  CONST_MEMBER_FUNC(ASTListIteratorPair<Type>, parameterTypes);
  CONST_MEMBER_FUNC(Type*, returnType);
};
} // namespace node
} // namespace ast
#endif
