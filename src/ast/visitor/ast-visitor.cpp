#include "ast-visitor.h"

#include "ast/node/nodes.h"

#include <cassert>
#include <string>

namespace ast {
namespace visitor {
#define ast_node_def(type)                                                     \
  void ASTVisitor::visit(node::type* a) {                                      \
    if(!is##type##Node(a)) {                                                   \
      assert(false && "Unexpected " #type " in visitor ");                     \
    }                                                                          \
    visitImpl(a);                                                              \
  }
#include "ast/ast-node.inc"
} // namespace visitor
} // namespace ast
