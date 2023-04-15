#include "CallableType.h"
namespace ast {
namespace node {
NodeList* getParameterTypes(const NodeList& params) {
  auto types = new NodeList();
  for(auto p : params) {
    if(auto pt = toTypeNode(p); pt != nullptr) types->addBack(pt);
    else throw ASTException("Expected a Type");
  }
  return std::move(types);
}

CallableType* Closure::determineClosureType(
    Type* type_specifier,
    const NodeList& parameters,
    Type* returnType) {
  // if no callable type (PrimitiveTypeEnum::NIL) then construct from params
  // and returnType if callable type, number and type of paramers must match
  // exactly and return type must match ( if specified, otherwise we can infer
  // it)
  try {
    if(Type::isNilType(type_specifier))
      return new CallableType(getParameterTypes(parameters), returnType);
    CallableType* callableType = Type::toCallableType(type_specifier);

    if(callableType == nullptr) return nullptr;

    auto callableParamTypes = callableType->parameterTypes();
    auto paramTypes = getParameterTypes(parameters);
    bool paramTypesMatch = Type::isSameType(callableParamTypes, paramTypes);

    if(paramTypesMatch &&
       (Type::isNilType(returnType) ||
        Type::isSameType(returnType, callableType->returnType()))) {
      return callableType;
    } else {
      return nullptr;
    }
  } catch(const ASTException& exp) {
    return nullptr;
  }
}

} // namespace node
} // namespace ast
