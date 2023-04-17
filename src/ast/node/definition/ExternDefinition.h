#ifndef SAPPHIRE_AST_NODE_DEFINITION_EXTERNDEFINITION_H_
#define SAPPHIRE_AST_NODE_DEFINITION_EXTERNDEFINITION_H_
#include "ast/ast.h"

namespace ast {
namespace node {
class ExternDefinition : public ASTNode {
private:
  FunctionPrototype* functionPrototype_;

protected:
  virtual void replaceNode(ASTNode* old, ASTNode* replacement) override;

public:
  ExternDefinition(long line, FunctionPrototype* functionPrototype);
  ExternDefinition(FunctionPrototype* functionPrototype);
  virtual ~ExternDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  FunctionPrototype* functionPrototype();
};
} // namespace node
} // namespace ast
#endif
