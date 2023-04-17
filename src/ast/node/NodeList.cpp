#include "NodeList.h"
namespace ast {
namespace node {

void NodeList::replaceNode(ASTNode* old, ASTNode* replacement) {
  for(auto& elm : this->elms) {
    if(elm == old) {
      replacement->parent() = this;
      elm = replacement;
      break;
    }
  }
}

void NodeList::addFront(NodeType* elm) {
  elm->parent() = this;
  this->elms.push_front(elm);
}
void NodeList::addBack(NodeType* elm) {
  elm->parent() = this;
  this->elms.push_back(elm);
}

NodeList::NodeType* NodeList::get(std::size_t idx) const {
  return this->elms[idx];
}
std::size_t NodeList::size() const { return this->elms.size(); }
NodeList::const_iter_type NodeList::begin() const { return this->elms.begin(); }
NodeList::const_iter_type NodeList::end() const { return this->elms.end(); }

NodeList::iter_type NodeList::begin() { return this->elms.begin(); }
NodeList::iter_type NodeList::end() { return this->elms.end(); }

} // namespace node
} // namespace ast
