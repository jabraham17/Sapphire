#include "ASTNode.h"

#include "nodes.h"

namespace ast {
namespace node {
ASTNode::ASTNode() : line_(-1), parent_(nullptr){};
long ASTNode::line() { return this->line_; }
void ASTNode::setLine(long line) { this->line_ = line; }

void ASTNode::replaceWith(ASTNode* newNode) {
  // if no parent, nothing to update
  if(parent() != nullptr) parent()->replaceNode(this, newNode);
}

void ASTNode::replaceNode(ASTNode* old, ASTNode* replacement) {
  for(std::size_t idx = 0; idx < numChildren(); idx++) {
    if(child(idx) == old) {
      setChild(idx, replacement);
    }
  }
}

#define ast_node_def_exclude
#define ast_node_def_exclude_ASTNode
#define ast_node_def(type)                                                     \
  type* ASTNode::to##type() { return toNodeType<type>(this); }                 \
  bool ASTNode::is##type() { return isNodeType<type>(this); }
#include "ast/ast-node.inc"

} // namespace node
} // namespace ast
