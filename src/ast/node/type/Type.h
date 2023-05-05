#ifndef SAPPHIRE_AST_NODE_TYPE_TYPE_H_
#define SAPPHIRE_AST_NODE_TYPE_TYPE_H_

#include "ast/ast.h"

#include <array>
#include <iterator>
#include <type_traits>

namespace ast {
namespace node {

enum class PrimitiveTypeEnum {
  INT,
  UINT,
  REAL,
  STRING,
  BOOL,
  BYTE,
  NIL,
  UNTYPED,
  ANY,
  UNKNOWN
};

// todo: cleanup templates to be consistent

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
  virtual ASTNode* clone();

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

  CONST_MEMBER_FUNC(bool, isNilType)
  CONST_MEMBER_FUNC(bool, isUnknownType)
  CONST_MEMBER_FUNC(bool, isUntypedType)
  CONST_MEMBER_FUNC(bool, isAnyType)

  // get plain string using [a-zA-Z0-9_]
  CONST_MEMBER_FUNC(std::string, toMangledString)
  CONST_MEMBER_FUNC(std::string, toString)

  // same type, just the base
  static bool isSameBaseType(Type* t1, Type* t2);

  // same type, including nilable and ref
  static bool isSameType(Type* t1, Type* t2);

  static PrimitiveType* getNilType();
  static PrimitiveType* getAnyType();
  static PrimitiveType* getUntypedType();
  static PrimitiveType* getUnknownType();

  static PrimitiveType* getType(PrimitiveTypeEnum);
  static ArrayType* getArrayType(Type* elementType);

  // template <typename... Ts> static TupleType* getTupleType(Ts&&... types) {
  //   static_assert(
  //       std::conjunction<std::bool_constant<(
  //           (std::is_pointer_v<Ts> &&
  //            std::is_base_of_v<Type, std::remove_pointer_t<Ts>>),
  //           ...)>>::value,
  //       "");
  //   std::array<Type*, sizeof...(Ts)> arrayOfTypes;
  //   std::size_t idx = 0;
  //   (([&] {
  //      arrayOfTypes[idx] = types;
  //      idx++;
  //    }()),
  //    ...);
  //   return getTupleType(arrayOfTypes.begin(), arrayOfTypes.end());
  // }
  // template <typename InputIt>
  // static TupleType* getTupleType(InputIt typesBegin, InputIt typesEnd) {
  //   static_assert(
  //       std::is_same_v<
  //           typename std::iterator_traits<InputIt>::value_type,
  //           Type*>,
  //       "");
  //   TypeList ty(typesBegin, typesEnd);
  //   return getTupleType(&ty);
  // }
  // static TupleType* getTupleType(TypeList* types);

  template <typename... Ts>
  static CallableType* getCallableType(Type* returnType, Ts&&... types) {
    static_assert(
        std::conjunction<std::bool_constant<(
            (std::is_pointer_v<Ts> &&
             std::is_base_of_v<Type, std::remove_pointer_t<Ts>>),
            ...)>>::value,
        "");
    std::array<Type*, sizeof...(Ts)> arrayOfTypes;
    std::size_t idx = 0;
    (([&] {
       arrayOfTypes[idx] = types;
       idx++;
     }()),
     ...);
    return getCallableType(
        returnType,
        arrayOfTypes.begin(),
        arrayOfTypes.end());
  }
  template <typename InputIt>
  static CallableType* getCallableType(
      Type* returnType,
      InputIt parameterTypesBegin,
      InputIt parameterTypesEnd) {
    static_assert(
        std::is_same_v<
            typename std::iterator_traits<InputIt>::value_type,
            Type*>,
        "");
    ASTList ty(parameterTypesBegin, parameterTypesEnd);
    return getCallableType(returnType, &ty);
  }
  static CallableType*
  getCallableType(Type* returnType, const ASTList& parameterTypes);
  static ClassType* getClassType(const char* className);
};

} // namespace node
} // namespace ast
#endif
