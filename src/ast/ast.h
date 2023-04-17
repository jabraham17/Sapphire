#ifndef SAPPHIRE_AST_AST_H_
#define SAPPHIRE_AST_AST_H_

// only include the root
#include "ast/node/ASTNode.h"

#include <type_traits>

namespace ast {

namespace visitor {
class ASTVisitor;
}

namespace symbol {
class Symbol;
class FunctionSymbol;
} // namespace symbol

namespace node {
class ASTNode;
#define ast_node_def(type) class type;
#include "ast-node.inc"
} // namespace node

// defines the conversions
template <class T> T* toNodeType(node::ASTNode* a) {
  return dynamic_cast<T*>(a);
}
template <class T> bool isNodeType(node::ASTNode* a) {
  return toNodeType<T>(a) != nullptr;
}
#define ast_node_def(type)                                                     \
  node::type* to##type##Node(node::ASTNode* a);                                \
  bool is##type##Node(node::ASTNode* a);
#include "ast-node.inc"

} // namespace ast

#endif
