#ifndef SAPPHIRE_AST_NODE_DEFINITION_EXTERNDEFINITION_H_
#define SAPPHIRE_AST_NODE_DEFINITION_EXTERNDEFINITION_H_
#include "ast/ast.h"

namespace ast {
namespace node {
class ExternDefinition : public ASTNode {
private:
  std::size_t functionPrototypeIdx_;

public:
  ExternDefinition(long line, FunctionPrototype* functionPrototype);
  ExternDefinition(FunctionPrototype* functionPrototype);
  virtual ~ExternDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  // virtual ASTNode* clone() override;

  CONST_MEMBER_FUNC(FunctionPrototype*, functionPrototype)
};
} // namespace node
} // namespace ast
#endif
