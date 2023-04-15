#ifndef SAPPHIRE_AST_NODE_CLASSDEFINITION_H_
#define SAPPHIRE_AST_NODE_CLASSDEFINITION_H_
#include "ast/ast.h"
#include "ast/node/ASTNode.h"
namespace ast {
namespace node {
class ClassDefinition : public ASTNode {
  ClassType* classType_;
  NodeList* variables_;
  NodeList* functions_;
  NodeList* initializers_;
  InitDefinition* deinitializer_;

public:
  ClassDefinition(long lineNumber, ClassType* classType);
  ClassDefinition(ClassType* classType);
  virtual ~ClassDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  static ClassDefinition* buildClass(ClassType* classType, NodeList* defs);

  ClassType* classType();
  NodeList* variables();
  NodeList* functions();
  NodeList* initializers();
  InitDefinition* deinitializer();
};
} // namespace node
} // namespace ast
#endif
