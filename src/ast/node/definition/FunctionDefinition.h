#ifndef SAPPHIRE_AST_NODE_FUNCTIONDEFINITION_H_
#define SAPPHIRE_AST_NODE_FUNCTIONDEFINITION_H_
#include "ast/ast.h"
#include "ast/node/ASTNode.h"
namespace ast {
namespace node {
class FunctionDefinition : public ASTNode {
private:
  FunctionPrototype* functionPrototype_;
  Scope* body_;

public:
  FunctionDefinition(
      long lineNumber,
      FunctionPrototype* functionPrototype,
      Scope* body)
      : FunctionDefinition(functionPrototype, body) {
    setLine(lineNumber);
  }
  FunctionDefinition(FunctionPrototype* functionPrototype, Scope* body)
      : functionPrototype_(functionPrototype), body_(body) {}
  virtual ~FunctionDefinition() = default;
  // virtual std::string toString() override;
  virtual void accept(visitor::ASTVisitor* ast) override;

  FunctionPrototype* functionPrototype() { return functionPrototype_; }
  Scope* body() { return body_; }
};
} // namespace node
} // namespace ast
#endif
