#ifndef SAPPHIRE_AST_NODE_DEFINITION_PARAMETER_H_
#define SAPPHIRE_AST_NODE_DEFINITION_PARAMETER_H_
#include "ast/ast.h"
#include "ast/node/ASTNode.h"
namespace ast {
namespace node {
class Parameter : public ASTNode {
private:
  symbol::Symbol* symbol_;

public:
  Parameter(long lineNumber, symbol::Symbol* symbol, Type* type)
      : Parameter(symbol, type) {
    setLine(lineNumber);
  }
  Parameter(symbol::Symbol* symbol, Type* type);
  virtual ~Parameter() = default;
  symbol::Symbol* symbol() { return symbol_; }
  // virtual std::string toString() override;
  virtual void accept(visitor::ASTVisitor* ast) override;
};
} // namespace node
} // namespace ast
#endif
