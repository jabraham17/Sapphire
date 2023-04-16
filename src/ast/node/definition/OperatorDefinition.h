#ifndef SAPPHIRE_AST_NODE_DEFINITION_OPERATORDEFINITION_H_
#define SAPPHIRE_AST_NODE_DEFINITION_OPERATORDEFINITION_H_
#include "ast/ast.h"
#include "ast/node/OperatorType.h"

namespace ast {
namespace node {
class OperatorDefinition : public ASTNode {
  OperatorType op_;
  NodeList* parameters_;
  Scope* body_;

public:
  OperatorDefinition(
      long lineNumber,
      OperatorType op,
      NodeList* parameters,
      Scope* body)
      : OperatorDefinition(op, parameters, body) {
    setLine(lineNumber);
  }
  OperatorDefinition(OperatorType op, NodeList* parameters, Scope* body)
      : op_(op), parameters_(parameters), body_(body) {}
  virtual ~OperatorDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  OperatorType opType() { return op_; }
  NodeList* parameters() { return parameters_; }
  Scope* body() { return body_; }
};
} // namespace node
} // namespace ast
#endif
