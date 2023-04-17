#include "TypeList.h"

#include "Type.h"

#include <type_traits>
namespace ast {
namespace node {

ASTNode* TypeList::clone() {
  auto x = new TypeList();
  for(auto elm : this->types) {
    x->addBack(toNodeType<std::remove_pointer_t<decltype(elm)>>(elm->clone()));
  }
  return x;
}

void TypeList::replaceNode(ASTNode* old, ASTNode* replacement) {
  for(list_elm_type& elm : this->types) {
    if(elm == old) {
      replacement->parent() = this;
      // perform conversion
      auto newElm =
          toNodeType<std::remove_pointer_t<list_elm_type>>(replacement);
      elm = newElm;
      break;
    }
  }
}

TypeList::list_type TypeList::elementTypes() { return types; }
void TypeList::addBack(list_elm_type elm) {
  elm->parent() = this;
  types.push_back(elm);
}
void TypeList::addFront(list_elm_type elm) {
  elm->parent() = this;
  types.push_front(elm);
}

} // namespace node
} // namespace ast
