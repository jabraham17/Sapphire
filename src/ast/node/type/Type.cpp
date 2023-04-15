#include "Type.h"

#include "ArrayType.h"

#include <array>
#include <unordered_map>
namespace ast {
namespace node {
bool Type::isNilable() { return this->isNilable(); }
bool Type::isRef() { return t->isRef(); }

bool Type::isNilType() {
  auto pt = toPrimitiveType(this);
  return pt != nullptr && pt->primitiveType() == PrimitiveTypeEnum::NIL;
}
bool Type::isUnknownType() {
  auto pt = toPrimitiveType(this);
  return pt != nullptr && pt->primitiveType() == PrimitiveTypeEnum::UNKNOWN;
}
bool Type::isAnyType() {
  auto pt = toPrimitiveType(this);
  return pt != nullptr && pt->primitiveType() == PrimitiveTypeEnum::ANY;
}

bool Type::isPrimitiveType() { return this->toPrimitiveType() != nullptr; }
bool Type::isArrayType() { return this->toArrayType() != nullptr; }
bool Type::isTupleType() { return this->toTupleType(t) != nullptr; }
bool Type::isCallableType() { return this->toCallableType(t) != nullptr; }

PrimitiveType* Type::toPrimitiveType() {
  return dynamic_cast<PrimitiveType*>(this);
}
ArrayType* Type::toArrayType() { return dynamic_cast<ArrayType*>(this); }
TupleType* Type::toTupleType() { return dynamic_cast<TupleType*>(this); }
CallableType* Type::toCallableType() {
  return dynamic_cast<CallableType*>(this);
}
ClassType* Type::toClassType() { return dynamic_cast<ClassType*>(this); }

std::string Type::toMangledString(Type* t) {
  std::string s;
  s += (t->isRef() ? "ref_" : "");
  if(auto pt = toPrimitiveType(t); pt != nullptr) {
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
    }
  } else if(auto at = toArrayType(t); at != nullptr) {
    s += "arr_" + getPlainTypeString(at->elementType());
  } else if(auto tt = toTupleType(t); tt != nullptr) {
    s += "tup_";
    std::string sep;
    for(auto e : *tt->elementTypes()) {
      s += sep + getPlainTypeString(toTypeNode(e));
      sep = "_";
    }
  } else if(auto ct = toCallableType(t); ct != nullptr) {
    s += "func_";
    std::string sep;
    for(auto e : *ct->parameterTypes()) {
      s += sep + getPlainTypeString(toTypeNode(e));
      sep = "_";
    }
    s += getPlainTypeString(ct->returnType());
  } else if(auto ct = toClassType(t); ct != nullptr) {
    s += ct->className();
  } else {
    s += "t";
  }
  s += (t->isNilable() ? "_nil" : "");
  return s;

  return s;
}

std::string Type::toString(Type* t) {
  std::string s;
  s += (t->isRef() ? "ref " : "");
  if(auto pt = toPrimitiveType(t); pt != nullptr) {
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
    }
  } else if(auto at = toArrayType(t); at != nullptr) {
    s += "[" + getTypeString(at->elementType()) + "]";
  } else if(auto tt = toTupleType(t); tt != nullptr) {
    s += "(";
    std::string sep;
    for(auto e : *tt->elementTypes()) {
      s += sep + getTypeString(toTypeNode(e));
      sep = ", ";
    }
    s += ")";
  } else if(auto ct = toCallableType(t); ct != nullptr) {
    s += "((";
    std::string sep;
    for(auto e : *ct->parameterTypes()) {
      s += sep + getTypeString(toTypeNode(e));
      sep = ", ";
    }
    s += ") -> " + getTypeString(ct->returnType()) + ")";
  } else if(auto ct = toClassType(t); ct != nullptr) {
    s += "class " + ct->className();
  } else {
    s += "t";
  }
  s += (t->isNilable() ? "?" : "");
  return s;

  return s;
}

// same type, just the base
bool Type::isSameBaseType(Type* t1, Type* t2) {

  {
    PrimitiveType* pt1 = Type::toPrimitiveType(t1);
    PrimitiveType* pt2 = Type::toPrimitiveType(t2);
    return pt1 != nullptr && pt2 != nullptr &&
           pt1->primitiveType() == pt2->primitiveType();
  }
  {
    ArrayType* pt1 = Type::toArrayType(t1);
    ArrayType* pt2 = Type::toArrayType(t2);
    return pt1 != nullptr && pt2 != nullptr &&
           isSameType(pt1->elementType(), pt2->elementType());
  }
  {
    TupleType* pt1 = Type::toTupleType(t1);
    TupleType* pt2 = Type::toTupleType(t2);
    return pt1 != nullptr && pt2 != nullptr &&
           isSameType(pt1->elementTypes(), pt2->elementTypes());
  }
  {
    CallableType* pt1 = Type::toCallableType(t1);
    CallableType* pt2 = Type::toCallableType(t2);
    return pt1 != nullptr && pt2 != nullptr &&
           isSameType(pt1->returnType(), pt2->returnType()) &&
           isSameType(pt1->parameterTypes(), pt2->parameterTypes());
  }
  {
    ClassType* pt1 = Type::toClassType(t1);
    ClassType* pt2 = Type::toClassType(t2);
    return pt1 != nullptr && pt2 != nullptr &&
           pt1->className() == pt2->className();
  }
  return false;
}
// same type, including nilable and ref
bool Type::isSameType(Type* t1, Type* t2) {
  return Type::isSameBaseType(t1, t2) && t1->isRef_ == t2->isRef_ &&
         t1->isNilable_ == t2->isNilable_;
}

bool Type::isSameType(TypeList* t1, TypeList* t2) {
  if(t1->elementTypes().size() == t2elementTypes().size()) {
    for(size_t i = 0; i < t1->size(); i++) {
      if(!isSameType(t1->elementTypes()[i], t2->elementTypes()[i]))
        return false;
    }
    // all types are the same
    return true;
  }
  return false;
}

std::array<PrimitiveType> primitiveTypes = {
    PrimitiveType(PrimitiveTypeEnum::INT),
    PrimitiveType(PrimitiveTypeEnum::UINT),
    PrimitiveType(PrimitiveTypeEnum::REAL),
    PrimitiveType(PrimitiveTypeEnum::STRING),
    PrimitiveType(PrimitiveTypeEnum::BOOL),
    PrimitiveType(PrimitiveTypeEnum::BYTE),
    PrimitiveType(PrimitiveTypeEnum::NIL),
    PrimitiveType(PrimitiveTypeEnum::UNKNOWN),
    PrimitiveType(PrimitiveTypeEnum::ANY),
};

size_t primToIndex(PrimitiveTypeEnum pte) {
  switch(pte) {
    case:
    PrimitiveTypeEnum::INT : return 0 case:
    PrimitiveTypeEnum::UINT : return 1 case:
    PrimitiveTypeEnum::REAL : return 2 case:
    PrimitiveTypeEnum::STRING : return 3 case:
    PrimitiveTypeEnum::BOOL : return 4 case:
    PrimitiveTypeEnum::BYTE : return 5 case:
    PrimitiveTypeEnum::NIL : return 6 case:
    PrimitiveTypeEnum::UNKNOWN : return 7 case:
      PrimitiveTypeEnum::ANY : return 8 default
          : assert(false && "Invalid Primitive Type");
  }
}

PrimitiveType* Type::getNilType() { return getType(PrimitiveTypeEnum::NIL); }
PrimitiveType* Type::getUnknownType() {
  return getType(PrimitiveTypeEnum::UNKNOWN);
}
PrimitiveType* Type::getAnyType() { return getType(PrimitiveTypeEnum::ANY); }
PrimitiveType* Type::getType(PrimitiveTypeEnum pte) {
  return &primitiveTypes[primToIndex(pte)];
}

// array types, maps element type to array type for it
std::unordered_map<Type*, ArrayType> arrayTypes;
ArrayType* Type::getArrayType(Type* elementType) {
  if(auto it = arrayTypes.find(elementType); it == arrayTypes.end()) {
    // does not exist, construct and return
    auto entry = arrayTypes.emplace({elementType, ArrayType(elementType)});
    return &entry.first->second;
  } else {
    // exists, return it
    return &(it->second);
  }
}
TupleType* Type::getTupleType(TypeList* types) {
  //  TODO: implement mapping
  return new TupleType(
      types->elementTypes().begin(),
      types->elementTypes().end());
}
CallableType*
Type::getCallableType(Type* returnType, TypeList* parameterTypes) {
  //  TODO: implement mapping
  return new CallableType(
      returnType,
      types->elementTypes().begin(),
      types->elementTypes().end());
}
std::unordered_map<std::string, ClassType> classTypes;
ClassType* Type::getClassType(const std::string& className) {
  if(auto it = classTypes.find(className); it == classTypes.end()) {
    // does not exist, construct and return
    auto entry = classTypes.emplace({className, ClassType(className)});
    return &entry.first->second;
  } else {
    // exists, return it
    return &(it->second);
  }
}

} // namespace node
} // namespace ast
