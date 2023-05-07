#ifndef SAPPHIRE_AST_NODE_DEFINITION_FUNCTIONPROTOTYPE_H_
#define SAPPHIRE_AST_NODE_DEFINITION_FUNCTIONPROTOTYPE_H_
#include "ast/ast.h"

#include <string>

namespace ast {
namespace node {
class FunctionPrototype : public ASTNode {
private:
  std::size_t parametersStartIdx_;
  std::size_t numParameters_;
  symbol::FunctionSymbol* funcSymbol_;
  bool isMangled_;

public:
  FunctionPrototype(
      long line,
      const char* name,
      const ASTList& parameters,
      Type* returnType,
      bool isMangled,
      Type* belongsTo);
  FunctionPrototype(
      const char* name,
      const ASTList& parameters,
      Type* returnType,
      bool isMangled,
      Type* belongsTo);

  virtual ~FunctionPrototype() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  // virtual ASTNode* clone() override;

  CONST_MEMBER_FUNC(std::string, name)
  CONST_MEMBER_FUNC(ASTListIteratorPair<Parameter>, parameters)
  CONST_MEMBER_FUNC(CallableType*, type)
  CONST_MEMBER_FUNC(symbol::FunctionSymbol*, symbol)
  CONST_MEMBER_FUNC(bool, isMangled)
};
} // namespace node
} // namespace ast
#endif
