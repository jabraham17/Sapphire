#ifndef SAPPHIRE_AST_NODE_DEFINITION_FUNCTIONDEFINITION_H_
#define SAPPHIRE_AST_NODE_DEFINITION_FUNCTIONDEFINITION_H_
#include "ast/ast.h"

namespace ast {
namespace node {
class FunctionDefinition : public ASTNode {
private:
  FunctionPrototype* functionPrototype_;
  Scope* body_;

protected:
  virtual void replaceNode(ASTNode* old, ASTNode* replacement) override;

public:
  FunctionDefinition(
      long line,
      FunctionPrototype* functionPrototype,
      Scope* body);
  FunctionDefinition(FunctionPrototype* functionPrototype, Scope* body);

  virtual ~FunctionDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  FunctionPrototype* functionPrototype();
  Scope* body();
};
} // namespace node
} // namespace ast
#endif
