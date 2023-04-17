#include "ast.h"

#include "ast/node/nodes.h"
#include "visitor/ast-visitor.h"

#include <cassert>

namespace ast {

// defines the conversions
#define ast_node_def(type)                                                     \
  node::type* to##type##Node(node::ASTNode* a) {                               \
    return toNodeType<node::type>(a);                                          \
  }                                                                            \
  bool is##type##Node(node::ASTNode* a) { return isNodeType<node::type>(a); }
#include "ast-node.inc"

// defines all the accepts
#define ast_node_def(type)                                                     \
  void node::type::accept(visitor::ASTVisitor* ast) { ast->visit(this); }
#include "ast-node.inc"

} // namespace ast
