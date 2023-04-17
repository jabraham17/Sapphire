#ifndef SAPPHIRE_AST_NODE_TYPE_TYPELIST_H_
#define SAPPHIRE_AST_NODE_TYPE_TYPELIST_H_

#include "ast/ast.h"

#include <deque>
namespace ast {
namespace node {
// TODO: make a specializeation of ASTList using CRTP
// todo: move to cpp definition stuff
// todo: cleanup templates to be consistent
class TypeList : public ASTNode {
public:
  using list_elm_type = Type*;
  using list_type = std::deque<list_elm_type>;

private:
  list_type types;

protected:
  virtual void replaceNode(ASTNode* old, ASTNode* replacement) override;

public:
  TypeList() : types() {}
  template <class InputIt>
  TypeList(long line, InputIt typesBegin, InputIt typesEnd)
      : TypeList(typesBegin, typesEnd) {
    setLine(line);
  }
  template <class InputIt>
  TypeList(InputIt typesBegin, InputIt typesEnd) : types() {
    // loop through and add each elm, this will update the parent
    for(auto it = typesBegin; it != typesEnd; it++) {
      addBack(*it);
    }
  }
  virtual ~TypeList() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  list_type elementTypes();
  void addBack(list_elm_type elm);
  void addFront(list_elm_type elm);
};
} // namespace node
} // namespace ast

#endif
