#ifndef SAPPHIRE_AST_NODE_ASTNODE_H_
#define SAPPHIRE_AST_NODE_ASTNODE_H_
#include "ast/ast.h"

#include <vector>

namespace ast {
namespace visitor {
class ASTVisitor;
}

namespace node {
class ASTNode {
private:
  long line_;

protected:
  ASTNode* parent_;

public:
  ASTNode();
  virtual ~ASTNode() = default;
  virtual void accept(visitor::ASTVisitor* ast) = 0;
  long line();
  void setLine(long line);

  void replaceWith(ASTNode* newNode) {
    // if no parent, nothing to update
    if(parent() != nullptr) parent()->replaceNode(this, newNode);
  }

  virtual ASTNode* clone() = 0;

  // return by ref, so we can change
  ASTNode*& parent() { return this->parent_; }

protected:
  // default does nothing, if the node has no children
  virtual void replaceNode(
      [[maybe_unused]] ASTNode* old,
      [[maybe_unused]] ASTNode* replacement){};
};
} // namespace node
} // namespace ast
#endif
