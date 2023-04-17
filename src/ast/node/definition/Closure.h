#ifndef SAPPHIRE_AST_NODE_DEFINITION_CLOSURE_H_
#define SAPPHIRE_AST_NODE_DEFINITION_CLOSURE_H_
#include "ast/ast.h"
#include "ast/node/expression/Expression.h"

namespace ast {
namespace node {
class Closure : public Expression {
  CallableType* type_;
  NodeList* parameters_;
  Scope* body_;

protected:
  virtual void replaceNode(ASTNode* old, ASTNode* replacement) override;

public:
  Closure(long line, CallableType* type, NodeList* parameters, Scope* body);
  Closure(CallableType* type, NodeList* parameters, Scope* body);
  virtual ~Closure() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  virtual Type* type() override;
  virtual void setType(Type* type) override;

  static CallableType* determineClosureType(
      Type* type_specifier,
      const NodeList& parameters,
      Type* returnType);

  NodeList* parameters();
  Scope* body();
};
} // namespace node
} // namespace ast
#endif
