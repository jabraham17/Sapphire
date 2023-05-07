#ifndef SAPPHIRE_AST_NODE_DEFINITION_OPERATORDEFINITION_H_
#define SAPPHIRE_AST_NODE_DEFINITION_OPERATORDEFINITION_H_
#include "ast/ast.h"
#include "ast/node/OperatorType.h"

namespace ast {
namespace node {
class OperatorDefinition : public ASTNode {
  OperatorType op_;
  std::size_t parametersStartIdx_;
  std::size_t numParameters_;
  std::size_t bodyIdx_;

public:
  OperatorDefinition(
      long line,
      OperatorType op,
      const ASTList& parameters,
      Scope* body);

  OperatorDefinition(OperatorType op, const ASTList& parameters, Scope* body);

  virtual ~OperatorDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  // virtual ASTNode* clone() override;

  CONST_MEMBER_FUNC(OperatorType, opType);
  CONST_MEMBER_FUNC(ASTListIteratorPair<Parameter>, parameters);
  CONST_MEMBER_FUNC(Scope*, body);
};
} // namespace node
} // namespace ast
#endif
