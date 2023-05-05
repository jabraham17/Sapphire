#include "FunctionPrototype.h"

#include "Parameter.h"

#include "ast/node/type/CallableType.h"
#include "ast/symbol/function-symbol.h"
#include "ast/symbol/symbol.h"

#include <cassert>

namespace ast {
namespace node {

// ASTNode* FunctionPrototype::clone() {
//   assert(false && "function prototype clone is unimplemented");
//   return nullptr;
// }

FunctionPrototype::FunctionPrototype(
    long line,
    const char* name,
    const ASTList& parameters,
    Type* returnType,
    bool isMangled,
    Type* belongsTo)
    : FunctionPrototype(name, parameters, returnType, isMangled, belongsTo) {
  setLine(line);
}
FunctionPrototype::FunctionPrototype(
    const char* name,
    const ASTList& parameters,
    Type* returnType,
    bool isMangled,
    Type* belongsTo)
    : funcSymbol_(nullptr), isMangled_(isMangled) {

  this->parametersStartIdx_ = this->numChildren();
  // add a this parameter
  bool hasThisArg = false;
  if(!belongsTo->isUntypedType() && !belongsTo->isNilType() &&
     !belongsTo->isAnyType()) {
    this->addChild(new Parameter(new symbol::Symbol("this"), belongsTo));
    hasThisArg = true;
  }
  for(auto p : parameters) {
    this->addChild(p);
  }
  this->parametersStopIdx_ =
      this->parametersStartIdx_ + parameters.size() + int(hasThisArg);

  ASTList parameterTypes;
  for(auto p : this->parameters()) {
    parameterTypes.push_back(p->symbol()->type()->clone());
  }
  Type* funcSymbolType = new CallableType(parameterTypes, returnType);
  funcSymbol_ =
      new symbol::FunctionSymbol(this, name, belongsTo, funcSymbolType);
}

std::string FunctionPrototype::name() { return this->symbol()->name(); }
ASTListIteratorPair<Parameter> FunctionPrototype::parameters() {
  return children_slice<Parameter>(
      this->parametersStartIdx_,
      this->parametersStopIdx_);
}
CallableType* FunctionPrototype::type() {
  auto type_ = funcSymbol_->type()->toCallableType();
  assert(type_ != nullptr && "functions must have callable types");
  return type_;
}
symbol::FunctionSymbol* FunctionPrototype::symbol() {
  return this->funcSymbol_;
}

bool FunctionPrototype::isMangled() { return this->isMangled_; }
} // namespace node
} // namespace ast
