#ifndef SAPPHIRE_AST_NODE_STATEMENT_SCOPE_H_
#define SAPPHIRE_AST_NODE_STATEMENT_SCOPE_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {

// todo: move to cpp definition stuff

class Scope : public Statement {
  NodeList* statements_;

protected:
  virtual void replaceNode(ASTNode* old, ASTNode* replacement) override;

public:
  Scope(long line, NodeList* statements);
  Scope(NodeList* statements);
  virtual ~Scope() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual ASTNode* clone() override;

  NodeList* statements();
};
} // namespace node
} // namespace ast
#endif
