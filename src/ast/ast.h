#ifndef SAPPHIRE_AST_AST_H_
#define SAPPHIRE_AST_AST_H_

namespace ast {

namespace visitor {
class ASTVisitor;
}

namespace symbol {
class Symbol;
class FunctionSymbol;
} // namespace symbol

namespace node {
class ASTNode
#define ast_node_def(type) class type;
#include "ast-node.inc"
} // namespace node

// defines the conversions
#define ast_node_def(type)                                                     \
  node::type* to##type##Node(node::ASTNode* a);                                \
  bool is##type##Node(node::ASTNode* a);
#include "ast-node.inc"

} // namespace ast

#endif
