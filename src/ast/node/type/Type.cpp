#include "Type.h"

#include "ArrayType.h"
#include "CallableType.h"
#include "ClassType.h"
#include "PrimitiveType.h"
// #include "TupleType.h"

#include <array>
#include <cassert>
#include <string>
#include <unordered_map>
namespace ast {
namespace node {

ASTNode* Type::clone() {
  auto x = new Type(isRef_, isNilable_);
  x->isUserSpecified_ = isUserSpecified_;
  return x;
}

bool Type::isNilType() {
  auto pt = this->toPrimitiveType();
  return pt != nullptr && pt->primitiveType() == PrimitiveTypeEnum::NIL;
}
bool Type::isUnknownType() {
  auto pt = this->toPrimitiveType();
  return pt != nullptr && pt->primitiveType() == PrimitiveTypeEnum::UNKNOWN;
}
bool Type::isAnyType() {
  auto pt = this->toPrimitiveType();
  return pt != nullptr && pt->primitiveType() == PrimitiveTypeEnum::ANY;
}
bool Type::isUntypedType() {
  auto pt = this->toPrimitiveType();
  return pt != nullptr && pt->primitiveType() == PrimitiveTypeEnum::UNTYPED;
}

std::string Type::toMangledString() {
  std::string s;
  s += (this->isRef() ? "ref_" : "");
  if(auto pt = this->toPrimitiveType(); pt != nullptr) {
    switch(pt->primitiveType()) {
      case PrimitiveTypeEnum::INT: s += "int"; break;
      case PrimitiveTypeEnum::UINT: s += "uint"; break;
      case PrimitiveTypeEnum::REAL: s += "real"; break;
      case PrimitiveTypeEnum::STRING: s += "str"; break;
      case PrimitiveTypeEnum::BOOL: s += "bool"; break;
      case PrimitiveTypeEnum::BYTE: s += "byte"; break;
      case PrimitiveTypeEnum::NIL: s += "nil"; break;
      case PrimitiveTypeEnum::UNKNOWN: s += "unknown"; break;
      case PrimitiveTypeEnum::ANY: s += "any"; break;
      case PrimitiveTypeEnum::UNTYPED: s += "untyped"; break;
    }
  } else if(auto at = this->toArrayType(); at != nullptr) {
    s += "arr_" + at->elementType()->toString();
  }
  // else if(auto tt = this->toTupleType(); tt != nullptr) {
  //   s += "tup_";
  //   std::string sep;
  //   for(auto e : tt->elementTypes()->elementTypes()) {
  //     s += sep + e->toType()->toString();
  //     sep = "_";
  //   }
  // }
  else if(auto ct = this->toCallableType(); ct != nullptr) {
    s += "func_";
    std::string sep;
    for(auto e : ct->parameterTypes()) {
      s += sep + e->toString();
      sep = "_";
    }
    s += ct->returnType()->toString();
  } else if(auto ct = this->toClassType(); ct != nullptr) {
    s += ct->className();
  } else {
    s += "t";
  }
  s += (this->isNilable() ? "_nil" : "");
  return s;

  return s;
}

std::string Type::toString() {
  std::string s;
  s += (this->isRef() ? "ref " : "");
  if(auto pt = this->toPrimitiveType(); pt != nullptr) {
    switch(pt->primitiveType()) {
      case PrimitiveTypeEnum::INT: s += "int"; break;
      case PrimitiveTypeEnum::UINT: s += "uint"; break;
      case PrimitiveTypeEnum::REAL: s += "real"; break;
      case PrimitiveTypeEnum::STRING: s += "string"; break;
      case PrimitiveTypeEnum::BOOL: s += "bool"; break;
      case PrimitiveTypeEnum::BYTE: s += "byte"; break;
      case PrimitiveTypeEnum::NIL: s += "nil"; break;
      case PrimitiveTypeEnum::UNKNOWN: s += "unknown"; break;
      case PrimitiveTypeEnum::ANY: s += "any"; break;
      case PrimitiveTypeEnum::UNTYPED: s += "untyped"; break;
    }
  } else if(auto at = this->toArrayType(); at != nullptr) {
    s += "[" + at->elementType()->toString() + "]";
  }
  // else if(auto tt = this->toTupleType(); tt != nullptr) {
  //   s += "(";
  //   std::string sep;
  //   for(auto e : tt->elementTypes()->elementTypes()) {
  //     s += sep + e->toString();
  //     sep = ", ";
  //   }
  //   s += ")";
  // }
  else if(auto ct = this->toCallableType(); ct != nullptr) {
    s += "((";
    std::string sep;
    for(auto e : ct->parameterTypes()) {
      s += sep + e->toString();
      sep = ", ";
    }
    s += ") -> " + ct->returnType()->toString() + ")";
  } else if(auto ct = this->toClassType(); ct != nullptr) {
    s += "class " + ct->className();
  } else {
    s += "t";
  }
  s += (this->isNilable() ? "?" : "");
  return s;

  return s;
}

bool isSameType(ASTListIteratorPair<Type> t1, ASTListIteratorPair<Type> t2) {
  return t1.size() == t2.size() &&
         std::equal(
             t1.begin(),
             t2.end(),
             t1.begin(),
             [](auto t1_elm, auto t2_elm) {
               return Type::isSameType(t1_elm, t2_elm);
             });
}

// same type, just the base
bool Type::isSameBaseType(Type* t1, Type* t2) {

  {
    PrimitiveType* pt1 = t1->toPrimitiveType();
    PrimitiveType* pt2 = t2->toPrimitiveType();
    if(pt1 != nullptr && pt2 != nullptr)
      return pt1->primitiveType() == pt2->primitiveType();
  }
  {
    ArrayType* pt1 = t1->toArrayType();
    ArrayType* pt2 = t2->toArrayType();
    if(pt1 != nullptr && pt2 != nullptr)
      return isSameType(pt1->elementType(), pt2->elementType());
  }
  // {
  //   TupleType* pt1 = t1->toTupleType();
  //   TupleType* pt2 = t2->toTupleType();
  //   if(pt1 != nullptr && pt2 != nullptr)
  //     return isSameType(pt1->elementTypes(), pt2->elementTypes());
  // }
  {
    CallableType* pt1 = t1->toCallableType();
    CallableType* pt2 = t2->toCallableType();
    if(pt1 != nullptr && pt2 != nullptr)
      return isSameType(pt1->returnType(), pt2->returnType()) &&
             ast::node::isSameType(
                 pt1->parameterTypes(),
                 pt2->parameterTypes());
  }
  {
    ClassType* pt1 = t1->toClassType();
    ClassType* pt2 = t2->toClassType();
    if(pt1 != nullptr && pt2 != nullptr)
      return pt1->className() == pt2->className();
  }
  return false;
}
// same type, including nilable and ref
bool Type::isSameType(Type* t1, Type* t2) {
  return Type::isSameBaseType(t1, t2) && t1->isRef_ == t2->isRef_ &&
         t1->isNilable_ == t2->isNilable_;
}

std::array<PrimitiveType, 10> primitiveTypes = {
    PrimitiveType(PrimitiveTypeEnum::INT),
    PrimitiveType(PrimitiveTypeEnum::UINT),
    PrimitiveType(PrimitiveTypeEnum::REAL),
    PrimitiveType(PrimitiveTypeEnum::STRING),
    PrimitiveType(PrimitiveTypeEnum::BOOL),
    PrimitiveType(PrimitiveTypeEnum::BYTE),
    PrimitiveType(PrimitiveTypeEnum::NIL),
    PrimitiveType(PrimitiveTypeEnum::UNKNOWN),
    PrimitiveType(PrimitiveTypeEnum::ANY),
    PrimitiveType(PrimitiveTypeEnum::UNTYPED),
};

size_t primToIndex(PrimitiveTypeEnum pte) {
  switch(pte) {
    case PrimitiveTypeEnum::INT: return 0;
    case PrimitiveTypeEnum::UINT: return 1;
    case PrimitiveTypeEnum::REAL: return 2;
    case PrimitiveTypeEnum::STRING: return 3;
    case PrimitiveTypeEnum::BOOL: return 4;
    case PrimitiveTypeEnum::BYTE: return 5;
    case PrimitiveTypeEnum::NIL: return 6;
    case PrimitiveTypeEnum::UNKNOWN: return 7;
    case PrimitiveTypeEnum::ANY: return 8;
    case PrimitiveTypeEnum::UNTYPED: return 9;
    default: assert(false && "Invalid Primitive Type");
  }
}

PrimitiveType* Type::getNilType() { return getType(PrimitiveTypeEnum::NIL); }
PrimitiveType* Type::getUnknownType() {
  return getType(PrimitiveTypeEnum::UNKNOWN);
}
PrimitiveType* Type::getAnyType() { return getType(PrimitiveTypeEnum::ANY); }
PrimitiveType* Type::getUntypedType() {
  return getType(PrimitiveTypeEnum::UNTYPED);
}
PrimitiveType* Type::getType(PrimitiveTypeEnum pte) {
  return &primitiveTypes[primToIndex(pte)];
}

// array types, maps element type to array type for it
std::unordered_map<Type*, ArrayType> arrayTypes;
ArrayType* Type::getArrayType(Type* elementType) {
  if(auto it = arrayTypes.find(elementType); it == arrayTypes.end()) {
    // does not exist, construct and return
    auto entry =
        arrayTypes.emplace(std::make_pair(elementType, ArrayType(elementType)));
    return &entry.first->second;
  } else {
    // exists, return it
    return &(it->second);
  }
}
// TupleType* Type::getTupleType(TypeList* types) {
//   //  TODO: implement mapping
//   return new TupleType(
//       types->elementTypes().begin(),
//       types->elementTypes().end());
// }
CallableType*
Type::getCallableType(Type* returnType, const ASTList& parameterTypes) {
  //  TODO: implement mapping
  return new CallableType(parameterTypes, returnType);
}
std::unordered_map<std::string, ClassType> classTypes;
ClassType* Type::getClassType(const char* className) {
  if(auto it = classTypes.find(className); it == classTypes.end()) {
    // does not exist, construct and return
    auto entry =
        classTypes.emplace(std::make_pair(className, ClassType(className)));
    return &entry.first->second;
  } else {
    // exists, return it
    return &(it->second);
  }
}

} // namespace node
} // namespace ast
