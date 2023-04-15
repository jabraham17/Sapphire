#include "FunctionPrototype.h"
namespace ast {
namespace node {
FunctionPrototype::FunctionPrototype(
    const char* name,
    NodeList* parameters,
    Type* returnType,
    bool isMangled,
    Type* belongsTo)
    : belongsTo_(belongsTo), parameters_(parameters), funcSymbol_(nullptr),
      isMangled_(isMangled) {
  if(belongsTo) {
    parameters->addFront(new Parameter(new Symbol("this"), belongsTo));
  }
  NodeList* parameterTypes = new NodeList();
  // add a this parameter
  for(auto p : *parameters_) {
    auto pp = toParameterNode(p);
    assert(pp != nullptr);
    parameterTypes->addBack(pp->symbol()->type());
  }
  Type* funcSymbolType = new CallableType(parameterTypes, returnType);
  funcSymbol_ = new FunctionSymbol(this, name, funcSymbolType);
}

std::string FunctionSymbol::name() {
  std::string s;
  if(prototype()->isMangled_) s += "spp_";
  if(prototype()->belongsTo_ != nullptr)
    s += Type::getPlainTypeString(prototype()->belongsTo_) + "_";
  s += Symbol::name();
  return s;
}

std::string FunctionPrototype::name() { return symbol()->name(); }
CallableType* FunctionPrototype::type() {
  auto type_ = ast::toCallableTypeNode(funcSymbol_->type());
  assert(type_ != nullptr && "functions must have callable types");
  return type_;
}

} // namespace node
} // namespace ast
