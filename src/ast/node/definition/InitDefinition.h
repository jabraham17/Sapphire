#ifndef SAPPHIRE_AST_NODE_DEFINITION_INITDEFINITION_H_
#define SAPPHIRE_AST_NODE_DEFINITION_INITDEFINITION_H_
#include "ast/ast.h"

namespace ast {
namespace node {
class InitDefinition : public ASTNode {
private:
  std::size_t bodyIdx_;
  std::ptrdiff_t parametersStartIdx_;
  std::size_t numParameters_;
  bool isInit_;

public:
  InitDefinition(long line, const ASTList& parameters, Scope* body);
  InitDefinition(const ASTList& parameters, Scope* body);
  InitDefinition(long line, Scope* body);
  InitDefinition(Scope* body);
  virtual ~InitDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  // virtual ASTNode* clone() override;

  CONST_MEMBER_FUNC(bool, isDeinit)
  CONST_MEMBER_FUNC(bool, isInit)
  CONST_MEMBER_FUNC(ASTListIteratorPair<Parameter>, parameters)
  CONST_MEMBER_FUNC(Scope*, body)
};

} // namespace node
} // namespace ast
#endif
