#ifndef SAPPHIRE_AST_NODE_DEFINITION_EXTERNDEFINITION_H_
#define SAPPHIRE_AST_NODE_DEFINITION_EXTERNDEFINITION_H_
#include "ast/ast.h"
#include "ast/node/ASTNode.h"
namespace ast {
namespace node {
class ExternDefinition : public ASTNode {
private:
  FunctionPrototype* functionPrototype_;

public:
  ExternDefinition(long lineNumber, FunctionPrototype* functionPrototype)
      : ExternDefinition(functionPrototype) {
    setLine(lineNumber);
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
