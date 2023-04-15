#ifndef SAPPHIRE_AST_NODE_EXPRESSION_H_
#define SAPPHIRE_AST_NODE_EXPRESSION_H_
#include "ast/ast.h"
#include "ast/node/ASTNode.h"
namespace ast {
namespace node {
class Expression : public ASTNode {
public:
  Expression() = default;
  virtual ~Expression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  virtual Type* type() = 0;
  virtual void setType(Type* type) = 0;
};

} // namespace node
} // namespace ast
#endif
