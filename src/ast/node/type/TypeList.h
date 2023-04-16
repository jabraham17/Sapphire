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

public:
  TypeList() : types() {}
  template <class InputIt>
  TypeList(long line, InputIt typesBegin, InputIt typesEnd)
      : TypeList(typesBegin, typesEnd) {
    setLine(line);
  }
  template <class InputIt>
  TypeList(InputIt typesBegin, InputIt typesEnd)
      : types(typesBegin, typesEnd) {}
  virtual ~TypeList() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  list_type elementTypes() { return types; }
  void addBack(list_elm_type elm) { types.push_back(elm); }
  void addFront(list_elm_type elm) { types.push_front(elm); }
};
} // namespace node
} // namespace ast

#endif
