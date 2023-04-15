#ifndef SAPPHIRE_AST_NODE_TYPE_H_
#define SAPPHIRE_AST_NODE_TYPE_H_
#include "PrimitiveType.h"
#include "TypeList.h"

#include "ast/ast.h"
#include "ast/node/ASTNode.h"

#include <iterator_traits>
#include <type_traits>

namespace ast {
namespace node {

class Type : public ASTNode {
private:
  bool isRef_;
  bool isNilable_;
  bool isUserSpecified_;

public:
  Type(bool isRef, bool isNilable)
      : isRef_(isRef), isNilable_(isNilable), isUserSpecified_(false) {}
  virtual ~Type() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  bool isRef() { return isRef_; }
  bool isNilable() { return isNilable_; }
  bool isUserSpecified() { return isUserSpecified_; }

  virtual void setRef(bool isRef = true) { this->isRef_ = isRef; }
  virtual void setNilable(bool isNilable = true) {
    this->isNilable_ = isNilable;
  }
  void setUserSpecified(bool isUserSpecified = true) {
    this->isUserSpecified_ = isUserSpecified;
  }

  bool isNilable();
  bool isRef();

  bool isNilType();
  bool isUnknownType();
  bool isAnyType();

  bool isPrimitiveType();
  bool isArrayType();
  bool isTupleType();
  bool isCallableType();
  bool isClassType();

  PrimitiveType* toPrimitiveType();
  ArrayType* toArrayType();
  TupleType* toTupleType();
  CallableType* toCallableType();
  ClassType* toClassType();

  // get plain string using [a-zA-Z0-9_]
  std::string toMangledString(Type* t);
  static std::string toString(Type* t);

  // same type, just the base
  static bool isSameBaseType(Type* t1, Type* t2);
  // same type, including nilable and ref
  static bool isSameType(Type* t1, Type* t2);
  static bool isSameType(TypeList* t1, TypeList* t2);

  static PrimitiveType* getNilType();
  static PrimitiveType* getAnyType();
  static PrimitiveType* getUnknownType();

  static PrimitiveType* getType(PrimitiveTypeEnum);
  static ArrayType* getArrayType(Type* elementType);

  static ArrayType* getTypeList

      template <class... Ts>
      static TupleType* getTupleType(Ts&&... types) {
    static_assert(
        std::conjunction <
            std::bool_constant<(
                std::is_pointer_v<Ts> &&
                std::is_base_of_v<Type, std::remove_pointer_t<Ts.type()>>)...>::
                value,
        "");
    std::array<Type*, sizeof...(Ts)> arrayOfTypes;
    size_t idx = 0;
    (
        [&] {
          arrayOfTypes[idx] = types;
          id++;
        }(),
        ...);
    return getTupleType(arrayOfTypes.begin(), arrayOfTypes.end());
  }
  template <class InputIt>
  static TupleType* getTupleType(InputIt typesBegin, InputIt typesEnd) {
    static_assert(
        std::is_same_v<
            typename std::iterator_traits<InputIt>::value_type,
            Type*>,
        "");
    TypeList ty(typesBegin, typesEnd);
    return getTupleType(&ty);
  }
  static TupleType* getTupleType(TypeList* types);
  template <class... Ts>
  static TupleType* getCallableType(Type* returnType, Ts&&... types) {
    static_assert(
        std::conjunction <
            std::bool_constant<(
                std::is_pointer_v<Ts> &&
                std::is_base_of_v<Type, std::remove_pointer_t<Ts.type()>>)...>::
                value,
        "");
    std::array<Type*, sizeof...(Ts)> arrayOfTypes;
    size_t idx = 0;
    (
        [&] {
          arrayOfTypes[idx] = types;
          id++;
        }(),
        ...);
    return getCallableType(
        returnType,
        arrayOfTypes.begin(),
        arrayOfTypes.end());
  }
  static CallableType* getCallableType(
      Type* returnType,
      InputIt parameterTypesBegin,
      InputIt parameterTypesEnd) {
    static_assert(
        std::is_same_v<
            typename std::iterator_traits<InputIt>::value_type,
            Type*>,
        "");
    TypeList ty(parameterTypesBegin, parameterTypesEnd);
    return getCallableType(returnType, &ty);
  }
  static CallableType*
  getCallableType(Type* returnType, TypeList* parameterTypes);
  static ClassType* getClassType(const std::string& className);
};

} // namespace node
} // namespace ast
#endif
