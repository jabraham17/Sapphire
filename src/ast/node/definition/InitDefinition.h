#ifndef SAPPHIRE_AST_NODE_DEFINITION_INITDEFINITION_H_
#define SAPPHIRE_AST_NODE_DEFINITION_INITDEFINITION_H_
#include "ast/ast.h"

namespace ast {
namespace node {
class InitDefinition : public ASTNode {
private:
  NodeList* parameters_;
  Scope* body_;
  bool isInit_;

protected:
  virtual void replaceNode(ASTNode* old, ASTNode* replacement) override;

public:
  InitDefinition(long line, NodeList* parameters, Scope* body);
  InitDefinition(NodeList* parameters, Scope* body);
  InitDefinition(long line, Scope* body);
  InitDefinition(Scope* body);
  virtual ~InitDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual ASTNode* clone() override;

  bool isDeinit();
  bool isInit();
  NodeList* parameters();
  Scope* body();
};

} // namespace node
} // namespace ast
#endif
