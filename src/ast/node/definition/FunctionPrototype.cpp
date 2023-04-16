#include "FunctionPrototype.h"

#include "Parameter.h"

#include "ast/node/NodeList.h"
#include "ast/node/type/CallableType.h"
#include "ast/node/type/TypeList.h"
#include "ast/symbol/function-symbol.h"
#include "ast/symbol/symbol.h"

#include <cassert>

namespace ast {
namespace node {
FunctionPrototype::FunctionPrototype(
    long lineNumber,
    const char* name,
    NodeList* parameters,
    Type* returnType,
    bool isMangled,
    Type* belongsTo)
    : FunctionPrototype(name, parameters, returnType, isMangled, belongsTo) {
  setLine(lineNumber);
}
FunctionPrototype::FunctionPrototype(
    const char* name,
    NodeList* parameters,
    Type* returnType,
    bool isMangled,
    Type* belongsTo)
    : belongsTo_(belongsTo), parameters_(parameters), funcSymbol_(nullptr),
      isMangled_(isMangled) {
  if(belongsTo) {
    parameters->addFront(new Parameter(new symbol::Symbol("this"), belongsTo));
  }
  TypeList* parameterTypes = new TypeList();
  // add a this parameter
  for(auto p : *parameters_) {
    auto pp = toParameterNode(p);
    assert(pp != nullptr);
    parameterTypes->addBack(pp->symbol()->type());
  }
  Type* funcSymbolType = new CallableType(parameterTypes, returnType);
  funcSymbol_ = new symbol::FunctionSymbol(this, name, funcSymbolType);
}

std::string FunctionPrototype::name() { return this->symbol()->name(); }
NodeList* FunctionPrototype::parameters() { return this->parameters_; }
CallableType* FunctionPrototype::type() {
  auto type_ = ast::toCallableTypeNode(funcSymbol_->type());
  assert(type_ != nullptr && "functions must have callable types");
  return type_;
}
symbol::FunctionSymbol* FunctionPrototype::symbol() {
  return this->funcSymbol_;
}

} // namespace node
} // namespace ast
