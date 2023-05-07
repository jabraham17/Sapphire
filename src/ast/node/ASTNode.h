#ifndef SAPPHIRE_AST_NODE_ASTNODE_H_
#define SAPPHIRE_AST_NODE_ASTNODE_H_

#include "ASTList.hpp"

#include <cassert>
#include <cstddef>

namespace ast {
namespace visitor {
class ASTVisitor;
}

namespace node {

#define ast_node_def(type) class type;
#include "ast/ast-node.inc"

class ASTNode {
private:
  long line_;
  ASTNode* parent_;
  ASTList children_;

public:
  ASTNode();
  virtual ~ASTNode() = default;
  virtual void accept(visitor::ASTVisitor* ast);
  long line();
  void setLine(long line);

  void replaceWith(ASTNode* newNode);
  // virtual ASTNode* clone() = 0;

  ASTNode* parent() { return this->parent_; }
  const ASTList& children() { return this->children_; }

  template <class T = ASTNode> auto children_begin(std::size_t offset = 0) {
    return ASTListIterator<T>(this->children_.begin() + offset);
  }

  template <class T = ASTNode> auto children_end() {
    return ASTListIterator<T>(this->children_.end());
  }

  template <class T = ASTNode>
  auto children_slice(std::size_t start, std::size_t num) {
    auto beginIT = children_begin<T>(start);
    auto endIT = beginIT + num;
    return ASTListIteratorPair<T>(beginIT, endIT);
  }

  std::size_t numChildren() { return this->children_.size(); }

  auto child(std::size_t idx) {
    assert(idx < numChildren());
    return this->children_.at(idx);
  }

  void setChild(std::size_t idx, ASTNode* child) {
    assert(idx < numChildren());
    child->parent_ = this;
    this->children_[idx] = child;
  }
  std::size_t addChild(ASTNode* child) {
    auto idx = numChildren();
    child->parent_ = this;
    this->children_.push_back(child);
    return idx;
  }
  ASTNode* removeChild(std::size_t idx) {
    assert(idx < numChildren());
    auto c = child(idx);
    c->parent_ = nullptr;
    this->children_.erase((children_begin() + idx).it());
    return c;
  }

#define ast_node_def_exclude
#define ast_node_def_exclude_ASTNode
#define ast_node_def(type)                                                     \
  type* to##type();                                                            \
  bool is##type();
#include "ast/ast-node.inc"

private:
  // default does nothing, if the node has no children
  void replaceNode(ASTNode* old, ASTNode* replacement);
};
} // namespace node
} // namespace ast
#endif
