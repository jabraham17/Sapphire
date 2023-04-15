#ifndef SAPPHIRE_AST_NODE_TYPE_TYPELIST_H_
#define SAPPHIRE_AST_NODE_TYPE_TYPELIST_H_

#include "ast/ast.h"
#include "ast/node/ASTNode.h"

#include <vector>
namespace ast {
namespace node {
class TypeList : public ASTNode {
  using list = std::vector<Type*>;

private:
  list types;

public:
  template <class InputIt>
  TypeList(long lineNumber, InputIt typesBegin, InputIt typesEnd)
      : types(typesBegin, typesEnd) {
    setLine(lineNumber);
  }
  virtual ~TypeList() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  list elementTypes() { return types; }
};
} // namespace node
} // namespace ast

#endif
