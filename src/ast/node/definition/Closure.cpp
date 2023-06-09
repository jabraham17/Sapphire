// #include "Closure.h"

// #include "ast/node/statement/Scope.h"
// #include "ast/node/type/CallableType.h"
// #include "ast/node/type/Type.h"

// #include <cassert>
// namespace ast {
// namespace node {

// ASTNode* Closure::clone() {
//   return new Closure(
//       toNodeType<std::remove_pointer_t<decltype(type_)>>(type_->clone()),
//       toNodeType<std::remove_pointer_t<decltype(parameters_)>>(
//           parameters_->clone()),
//       toNodeType<std::remove_pointer_t<decltype(body_)>>(body_->clone()));
// }

// Closure::Closure(
//     long line,
//     CallableType* type,
//     NodeList* parameters,
//     Scope* body)
//     : Closure(type, parameters, body) {
//   setLine(line);
// }
// Closure::Closure(CallableType* type, NodeList* parameters, Scope* body)
//     : type_(type), parameters_(parameters), body_(body) {
//   type_->parent() = this;
//   parameters_->parent() = this;
//   body_->parent() = this;
// }

// Type* Closure::type() { return child(this->typeIdx_)->toType(); }
// void Closure::setType(Type* type) {
//   auto callableType = type->toCallableType();
//   assert(callableType != nullptr && "type must be a callable type");
//   this->type_ = callableType;
//   this->type_->parent() = this;
// }

// // TODO: make this clone or it will destroy the parent
// TypeList* copyParameterTypes(const NodeList& params) {
//   auto types = new TypeList();
//   for(auto p : params) {
//     auto pt = p->clone()->toType();
//     assert(pt != nullptr);
//     types->addBack(pt);
//   }
//   return std::move(types);
// }
// CallableType* Closure::determineClosureType(
//     Type* type_specifier,
//     const NodeList& parameters,
//     Type* returnType) {
//   // if no callable type (PrimitiveTypeEnum::NIL) then construct from params
//   // and returnType if callable type, number and type of paramers must match
//   // exactly and return type must match ( if specified, otherwise we can
//   infer
//   // it)
//   if(type_specifier->isNilType())
//     return new CallableType(copyParameterTypes(parameters), returnType);
//   CallableType* callableType = type_specifier->toCallableType();

//   if(callableType == nullptr) return nullptr;

//   auto callableParamTypes = callableType->parameterTypes();
//   auto paramTypes = copyParameterTypes(parameters);
//   bool paramTypesMatch = Type::isSameType(callableParamTypes, paramTypes);

//   if(paramTypesMatch &&
//      (returnType->isNilType() ||
//       Type::isSameType(returnType, callableType->returnType()))) {
//     return callableType;
//   } else {
//     return nullptr;
//   }
// }

// NodeList* Closure ::parameters() { return
// child(this->parametersIdx_)->toNodeList(); } Scope* Closure ::body() { return
// child(this->bodyIdx_)->toScope(); } } // namespace node } // namespace ast
