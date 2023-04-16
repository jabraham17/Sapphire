#ifndef SAPPHIRE_AST_NODE_STATEMENT_STATEMENT_H_
#define SAPPHIRE_AST_NODE_STATEMENT_STATEMENT_H_
#include "ast/ast.h"

namespace ast {
namespace node {
class Statement : public ASTNode {

public:
  Statement() = default;
  virtual ~Statement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
};

} // namespace node
} // namespace ast
#endif
