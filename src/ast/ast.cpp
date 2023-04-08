#include "ast.h"

namespace ast {

Expression::Expression(PrimitiveTypeEnum type)
    : type_(new PrimitiveType(type)) {}

DefExpression::DefExpression(
    Type* type,
    Symbol* symbol,
    Expression* assignValue)
    : Expression(type), symbol(symbol), assignValue(assignValue) {}
DefExpression::DefExpression(Type* type, Symbol* symbol)
    : DefExpression(type, symbol, new Nil()) {}
DefExpression::DefExpression(Symbol* symbol, Expression* assignValue)
    : DefExpression(assignValue->type(), symbol, assignValue) {}
DefExpression::DefExpression(Symbol* symbol)
    : DefExpression(symbol, new Nil()) {}

bool Type::isNilable(Type* t) { return t->isNilable_; }
bool Type::isRef(Type* t) { return t->isRef_; }
bool Type::isNilType(Type* t) {
  auto pt = toPrimitiveType(t);
  return pt != nullptr && pt->primType == PrimitiveTypeEnum::NIL;
}
bool Type::isUnknownType(Type* t) {
  auto pt = toPrimitiveType(t);
  return pt != nullptr && pt->primType == PrimitiveTypeEnum::UNKNOWN;
}

bool Type::isPrimitiveType(Type* t) { return toPrimitiveType(t) != nullptr; }
bool Type::isArrayType(Type* t) { return toArrayType(t) != nullptr; }
bool Type::isTupleType(Type* t) { return toTupleType(t) != nullptr; }
bool Type::isCallableType(Type* t) { return toCallableType(t) != nullptr; }

PrimitiveType* Type::toPrimitiveType(Type* t) {
  return dynamic_cast<PrimitiveType*>(t);
}
ArrayType* Type::toArrayType(Type* t) { return dynamic_cast<ArrayType*>(t); }
TupleType* Type::toTupleType(Type* t) { return dynamic_cast<TupleType*>(t); }
CallableType* Type::toCallableType(Type* t) {
  return dynamic_cast<CallableType*>(t);
}
ClassType* Type::toClassType(Type* t) { return dynamic_cast<ClassType*>(t); }

// same type, just the base
bool Type::isSameBaseType(Type* t1, Type* t2) {

  {
    PrimitiveType* pt1 = Type::toPrimitiveType(t1);
    PrimitiveType* pt2 = Type::toPrimitiveType(t2);
    return pt1 != nullptr && pt2 != nullptr && pt1->primType == pt2->primType;
  }
  {
    ArrayType* pt1 = Type::toArrayType(t1);
    ArrayType* pt2 = Type::toArrayType(t2);
    return pt1 != nullptr && pt2 != nullptr &&
           isSameType(pt1->arrayType, pt2->arrayType);
  }
  {
    TupleType* pt1 = Type::toTupleType(t1);
    TupleType* pt2 = Type::toTupleType(t2);
    return pt1 != nullptr && pt2 != nullptr &&
           isSameType(pt1->tupleTypes, pt2->tupleTypes);
  }
  {
    CallableType* pt1 = Type::toCallableType(t1);
    CallableType* pt2 = Type::toCallableType(t2);
    return pt1 != nullptr && pt2 != nullptr &&
           isSameType(pt1->returnType, pt2->returnType) &&
           isSameType(pt1->parameterTypes, pt2->parameterTypes);
  }
  {
    ClassType* pt1 = Type::toClassType(t1);
    ClassType* pt2 = Type::toClassType(t2);
    return pt1 != nullptr && pt2 != nullptr &&
           pt1->className->name() == pt2->className->name();
  }
  return false;
}
// same type, including nilable and ref
bool Type::isSameType(Type* t1, Type* t2) {
  return Type::isSameBaseType(t1, t2) && t1->isRef_ == t2->isRef_ &&
         t1->isNilable_ == t2->isNilable_;
}

bool Type::isSameType(NodeList<Type>* t1, NodeList<Type>* t2) {
  if(t1->size() == t2->size()) {
    for(auto i = 0; i < t1->size(); i++) {
      if(!isSameType((*t1)[i], (*t2)[i])) return false;
    }
    // all types are the same
    return true;
  }
  return false;
}

Type* Type::getNilType() { return new PrimitiveType(PrimitiveTypeEnum::NIL); }
Type* Type::getUnknownType() {
  return new PrimitiveType(PrimitiveTypeEnum::UNKNOWN);
}

ClassDefinition*
ClassDefinition::buildClass(Symbol* symbol, NodeList<ASTNode>* defs) {

  auto c = new ClassDefinition(symbol);
  for(auto def : *defs) {
    if(isDefExpressionNode(def)) {
      c->variables->addElement(toDefExpressionNode(def));
    } else if(isFunctionDefinitionNode(def)) {
      c->functions->addElement(toFunctionDefinitionNode(def));
    } else if(isInitDefinitionNode(def)) {
      auto init = toInitDefinitionNode(def);
      if(init->isDeinit()) c->deinitializer = init;
      else c->initializers->addElement(init);
    }
  }
  return c;
}

} // namespace ast
