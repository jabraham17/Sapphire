#ifndef SAPPHIRE_AST_NODE_DEFINITION_CLASSDEFINITION_H_
#define SAPPHIRE_AST_NODE_DEFINITION_CLASSDEFINITION_H_
#include "ast/ast.h"

namespace ast {
namespace node {
class ClassDefinition : public ASTNode {
  std::size_t classTypeIdx_;
  std::vector<std::size_t> variablesIdx_;
  std::vector<std::size_t> functionsIdx_;
  std::vector<std::size_t> initializersIdx_;
  std::ptrdiff_t deinitializerIdx_;

public:
  ClassDefinition(long line, ClassType* classType);
  ClassDefinition(ClassType* classType);
  virtual ~ClassDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  // virtual ASTNode* clone() override;

  static ClassDefinition* buildClass(ClassType* classType, ASTList defs);

  CONST_MEMBER_FUNC(ClassType*, classType)
  CONST_MEMBER_FUNC(ASTListSlice<DefExpression>, variables)
  CONST_MEMBER_FUNC(ASTListSlice<FunctionDefinition>, functions)
  CONST_MEMBER_FUNC(ASTListSlice<InitDefinition>, initializers)
  CONST_MEMBER_FUNC(InitDefinition*, deinitializer)
};
} // namespace node
} // namespace ast
#endif
