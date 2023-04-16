#ifndef SAPPHIRE_AST_NODE_ASTNODE_H_
#define SAPPHIRE_AST_NODE_ASTNODE_H_
#include "ast/ast.h"

namespace ast {
namespace visitor {
class ASTVisitor;
}

namespace node {
class ASTNode {
private:
  long line_;

public:
  ASTNode();
  virtual ~ASTNode() = default;
  virtual void accept(visitor::ASTVisitor* ast) = 0;
  long line();
  void setLine(long line);
};
} // namespace node
} // namespace ast
#endif
