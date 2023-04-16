#ifndef SAPPHIRE_AST_NODE_DEFINITION_PARAMETER_H_
#define SAPPHIRE_AST_NODE_DEFINITION_PARAMETER_H_
#include "ast/ast.h"

namespace ast {
namespace node {
class Parameter : public ASTNode {
private:
  symbol::Symbol* symbol_;

public:
  Parameter(long line, symbol::Symbol* symbol, Type* type);
  Parameter(symbol::Symbol* symbol, Type* type);
  virtual ~Parameter() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  symbol::Symbol* symbol();
};
} // namespace node
} // namespace ast
#endif
