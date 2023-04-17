#ifndef SAPPHIRE_AST_NODE_DEFINITION_OPERATORDEFINITION_H_
#define SAPPHIRE_AST_NODE_DEFINITION_OPERATORDEFINITION_H_
#include "ast/ast.h"
#include "ast/node/OperatorType.h"

namespace ast {
namespace node {
class OperatorDefinition : public ASTNode {
  OperatorType op_;
  NodeList* parameters_;
  Scope* body_;

protected:
  virtual void replaceNode(ASTNode* old, ASTNode* replacement) override;

public:
  OperatorDefinition(
      long line,
      OperatorType op,
      NodeList* parameters,
      Scope* body);

  OperatorDefinition(OperatorType op, NodeList* parameters, Scope* body);

  virtual ~OperatorDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  OperatorType opType();
  NodeList* parameters();
  Scope* body();
};
} // namespace node
} // namespace ast
#endif
