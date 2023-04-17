#include "ClassDefinition.h"

#include "FunctionDefinition.h"
#include "InitDefinition.h"

#include "ast/node/NodeList.h"
#include "ast/node/expression/DefExpression.h"
#include "ast/node/statement/Scope.h"
#include "ast/node/type/ClassType.h"

#include <cassert>
namespace ast {
namespace node {

ASTNode* ClassDefinition::clone() {
  assert(false && "class def clone is unimplemented");
  return nullptr;
}

void ClassDefinition::replaceNode(ASTNode* old, ASTNode* replacement) {
  if(classType_ == old) {
    replacement->parent() = this;
    classType_ =
        toNodeType<std::remove_pointer_t<decltype(classType_)>>(replacement);
    return;
  }
  if(variables_ == old) {
    replacement->parent() = this;
    variables_ =
        toNodeType<std::remove_pointer_t<decltype(variables_)>>(replacement);
    return;
  }
  if(functions_ == old) {
    replacement->parent() = this;
    functions_ =
        toNodeType<std::remove_pointer_t<decltype(functions_)>>(replacement);
    return;
  }
  if(initializers_ == old) {
    replacement->parent() = this;
    initializers_ =
        toNodeType<std::remove_pointer_t<decltype(initializers_)>>(replacement);
    return;
  }
  if(deinitializer_ == old) {
    replacement->parent() = this;
    deinitializer_ =
        toNodeType<std::remove_pointer_t<decltype(deinitializer_)>>(
            replacement);
    return;
  }
}

ClassDefinition::ClassDefinition(long line, ClassType* classType)
    : ClassDefinition(classType) {
  setLine(line);
}
ClassDefinition::ClassDefinition(ClassType* classType)
    : classType_(classType), variables_(new NodeList()),
      functions_(new NodeList()), initializers_(new NodeList()),
      deinitializer_(nullptr) {
  classType_->parent() = this;
  variables_->parent() = this;
  functions_->parent() = this;
  initializers_->parent() = this;
  if(deinitializer_ != nullptr) deinitializer_->parent() = this;
}

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
    c->deinitializer_->parent() = c;
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
