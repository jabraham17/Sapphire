#include "ClassDefinition.h"
namespace ast {
namespace node {

ClassDefinition::ClassDefinition(long lineNumber, ClassType* classType)
    : ClassDefinition(classType) {
  setLine(lineNumber);
}
ClassDefinition::ClassDefinition(ClassType* classType)
    : classType_(classType), variables_(new NodeList()),
      functions_(new NodeList()), initializers_(new NodeList()),
      deinitializer_(nullptr) {}

ClassDefinition*
ClassDefinition::buildClass(ClassType* classType, NodeList* defs) {

  auto c = new ClassDefinition(classType);
  for(auto def : *defs) {
    if(isDefExpressionNode(def)) {
      c->variables_->addBack(toDefExpressionNode(def));
    } else if(isFunctionDefinitionNode(def)) {
      c->functions_->addBack(toFunctionDefinitionNode(def));
    } else if(isInitDefinitionNode(def)) {
      auto init = toInitDefinitionNode(def);
      if(init->isDeinit()) c->deinitializer_ = init;
      else c->initializers_->addBack(init);
    }
  }
  if(c->deinitializer_ == nullptr) {
    c->deinitializer_ = new InitDefinition(new Scope(new NodeList()));
  }
  return c;
}

ClassType* ClassDefinition::classType() { return classType_; }
NodeList* ClassDefinition::variables() { return variables_; }
NodeList* ClassDefinition::functions() { return functions_; }
NodeList* ClassDefinition::initializers() { return initializers_; }
InitDefinition* ClassDefinition::deinitializer() { return deinitializer_; }

} // namespace node
} // namespace ast
