#include "NodeList.h"
namespace ast {
namespace node {

void NodeList::addFront(ASTNode* elm) { elms.push_front(elm); }
void NodeList::addBack(ASTNode* elm) { elms.push_back(elm); }

ASTNode* NodeList::get(size_t idx) { return elms[idx]; }
size_t NodeList::size() { return elms.size(); }
NodeList::iter_type NodeList::begin() { return elms.begin(); }
NodeList::iter_type NodeList::end() { return elms.end(); }
ASTNode*& NodeList::operator[](size_t idx) { return elms[idx]; }

} // namespace node
} // namespace ast
