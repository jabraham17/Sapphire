#include "ast.h"

#include "ast/node/nodes.h"
#include "visitor/ast-visitor.h"

#include <cassert>

namespace ast {

// defines all the accepts
#define ast_node_def(type)                                                     \
  void node::type::accept(visitor::ASTVisitor* ast) { ast->visit(this); }
#include "ast-node.inc"

} // namespace ast
