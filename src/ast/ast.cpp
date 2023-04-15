#include "ast.h"

#include "visitor/ast-visitor.h"

namespace ast {

// defines the conversions
#define ast_node_def(type)                                                     \
  type* to##type##Node(node::ASTNode* a) { return dynamic_cast<type*>(a); }    \
  bool is##type##Node(node::ASTNode* a) { return to##type##Node(a) != nullptr; }
#include "ast-node.inc"

// defines all the accepts
#define ast_node_def(type)                                                     \
  void node::type::accept(visitor::ASTVisitor* ast) { ast->visit(this); }
#include "ast-node.inc"

} // namespace ast
