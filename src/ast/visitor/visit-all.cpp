#include "ast/node/nodes.h"

#include "ast-visitor.h"

#define ast_node_def(type)                                                     \
  void ast::visitor::VisitAll::visitImpl(node::type* arg) {                    \
    for(auto c : arg->children()) {                                            \
      c->accept(this);                                                         \
    }                                                                          \
  }
#include "ast/ast-node.inc"
