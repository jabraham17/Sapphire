#ifndef SAPPHIRE_AST_NODE_DEFINITION_FUNCTIONDEFINITION_H_
#define SAPPHIRE_AST_NODE_DEFINITION_FUNCTIONDEFINITION_H_
#include "ast/ast.h"

namespace ast {
namespace node {
class FunctionDefinition : public ASTNode {
private:
  std::size_t functionPrototypeIdx_;
  std::size_t bodyIdx_;

public:
  FunctionDefinition(
      long line,
      FunctionPrototype* functionPrototype,
      Scope* body);
  FunctionDefinition(FunctionPrototype* functionPrototype, Scope* body);

  virtual ~FunctionDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  // virtual ASTNode* clone() override;

  CONST_MEMBER_FUNC(FunctionPrototype*, functionPrototype)
  CONST_MEMBER_FUNC(Scope*, body)
};
} // namespace node
} // namespace ast
#endif
