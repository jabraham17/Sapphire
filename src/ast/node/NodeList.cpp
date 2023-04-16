#include "NodeList.h"
namespace ast {
namespace node {

void NodeList::addFront(NodeType* elm) { this->elms.push_front(elm); }
void NodeList::addBack(NodeType* elm) { this->elms.push_back(elm); }

NodeList::NodeType* NodeList::get(std::size_t idx) const {
  return this->elms[idx];
}
std::size_t NodeList::size() const { return this->elms.size(); }
NodeList::const_iter_type NodeList::begin() const { return this->elms.begin(); }
NodeList::const_iter_type NodeList::end() const { return this->elms.end(); }
NodeList::NodeType* NodeList::operator[](std::size_t idx) const {
  return this->elms[idx];
}

NodeList::iter_type NodeList::begin() { return this->elms.begin(); }
NodeList::iter_type NodeList::end() { return this->elms.end(); }
NodeList::NodeType*& NodeList::operator[](std::size_t idx) {
  return this->elms[idx];
}

} // namespace node
} // namespace ast
