#include "ClassDefinition.h"

#include "FunctionDefinition.h"
#include "InitDefinition.h"

#include "ast/node/expression/DefExpression.h"
#include "ast/node/statement/Scope.h"
#include "ast/node/type/ClassType.h"

#include <cassert>
namespace ast {
namespace node {

// ASTNode* ClassDefinition::clone() {
//   assert(false && "class def clone is unimplemented");
//   return nullptr;
// }

ClassDefinition::ClassDefinition(long line, ClassType* classType)
    : ClassDefinition(classType) {
  setLine(line);
}
ClassDefinition::ClassDefinition(ClassType* classType) {
  this->classTypeIdx_ = addChild(classType);
  this->deinitializerIdx_ = -1;
}

ClassDefinition*
ClassDefinition::buildClass(ClassType* classType, ASTList defs) {

  auto c = new ClassDefinition(classType);
  for(auto def : defs) {
    if(def->isDefExpression()) {
      auto idx = c->addChild(def);
      c->variablesIdx_.push_back(idx);
    } else if(def->isFunctionDefinition()) {
      auto idx = c->addChild(def);
      c->functionsIdx_.push_back(idx);
    } else if(def->isInitDefinition()) {
      auto init = def->toInitDefinition();
      if(init->isDeinit()) {
        c->deinitializerIdx_ = c->addChild(def);
      } else {
        auto idx = c->addChild(def);
        c->initializersIdx_.push_back(idx);
      }
    }
  }
  if(c->deinitializerIdx_ == -1) {
    auto deinit = new InitDefinition(new Scope(ASTList()));
    c->deinitializerIdx_ = c->addChild(deinit);
  }
  return c;
}

ClassType* ClassDefinition::classType() {
  return child(this->classTypeIdx_)->toClassType();
}
ASTListSlice<DefExpression> ClassDefinition::variables() {
  return ASTListSlice<DefExpression>(
      this->children(),
      this->variablesIdx_.begin(),
      this->variablesIdx_.end());
}
ASTListSlice<FunctionDefinition> ClassDefinition::functions() {
  return ASTListSlice<FunctionDefinition>(
      this->children(),
      this->functionsIdx_.begin(),
      this->functionsIdx_.end());
}
ASTListSlice<InitDefinition> ClassDefinition::initializers() {
  return ASTListSlice<InitDefinition>(
      this->children(),
      this->initializersIdx_.begin(),
      this->initializersIdx_.end());
}
InitDefinition* ClassDefinition::deinitializer() {
  return child(this->deinitializerIdx_)->toInitDefinition();
}

} // namespace node
} // namespace ast
