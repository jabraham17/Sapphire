#ifndef SAPPHIRE_AST_NODE_DEFINITION_EXTERNDEFINITION_H_
#define SAPPHIRE_AST_NODE_DEFINITION_EXTERNDEFINITION_H_
#include "ast/ast.h"

namespace ast {
namespace node {
class ExternDefinition : public ASTNode {
private:
  FunctionPrototype* functionPrototype_;

public:
  ExternDefinition(long line, FunctionPrototype* functionPrototype)
      : ExternDefinition(functionPrototype) {
    setLine(line);
  }
  ExternDefinition(FunctionPrototype* functionPrototype)
      : functionPrototype_(functionPrototype) {}
  virtual ~ExternDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  FunctionPrototype* functionPrototype() { return functionPrototype_; }
};
} // namespace node
} // namespace ast
#endif
