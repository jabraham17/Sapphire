#include "ast.h"

#include "visitors/ast-visitor.h"

#include <cassert>
#include <iostream>

namespace ast {

Parameter::Parameter(Symbol* symbol, Type* type) : symbol_(symbol) {
  symbol_->setType(type);
}
//     std::string Parameter::toString() { return symbol_->name() + ": " +
//     symbol_->type()->toString(); }

// std::string FunctionPrototype::toString() {
//   std::string s;
//   if (namespaceName.has_value()) s += *namespaceName + ".";
//   s += name + "(";
//   std::string sep;
//   for(auto p: *parameters) {
//     s += sep + p->toString();
//     sep = ", ";
//   }
//   s += ") -> " + returnType->toString();
//   return s;
// }

//   std::string FunctionDefinition::toString() {
//     return
//   }

FunctionPrototype::FunctionPrototype(
    const char* name,
    NodeList* parameters,
    Type* returnType)
    : name_(name), parameters_(parameters), returnType_(returnType),
      shouldMangle_(false), funcSymbol_(nullptr) {
  NodeList* parameterTypes = new NodeList();
  for(auto p : *parameters_) {
    auto pp = toParameterNode(p);
    assert(pp != nullptr);
    parameterTypes->addBack(pp->symbol()->type());
  }
  auto funcSymbolType = new CallableType(parameterTypes, returnType_);
  funcSymbol_ = new Symbol(name, funcSymbolType);
}
FunctionPrototype::FunctionPrototype(
    const char* namespaceName,
    const char* name,
    NodeList* parameters,
    Type* returnType)
    : FunctionPrototype(name, parameters, returnType) {
  setNamespace(namespaceName);
}
void FunctionPrototype::setMangled(bool shouldMangle) {
  shouldMangle_ = shouldMangle;
  funcSymbol_->setName(this->mangledNamed());
}
void FunctionPrototype::setNamespace(const char* namespaceName) {
  this->namespaceName_ = namespaceName;
  funcSymbol_->setName(this->mangledNamed());
}

Expression::Expression(PrimitiveTypeEnum type)
    : type_(new PrimitiveType(type)) {}

DefExpression::DefExpression(
    Type* type,
    Symbol* symbol,
    Expression* assignValue)
    : Expression(type), symbol_(symbol), assignValue_(assignValue) {
  if(Type::isUnknownType(symbol_->type())) {
    symbol_->setType(type);
  }
}
DefExpression::DefExpression(Type* type, Symbol* symbol)
    : DefExpression(type, symbol, nullptr) {}
DefExpression::DefExpression(Symbol* symbol, Expression* assignValue)
    : DefExpression(assignValue->type(), symbol, assignValue) {}
DefExpression::DefExpression(Symbol* symbol) : DefExpression(symbol, nullptr) {}

bool DefExpression::hasInitialValue() {
  // auto n = toNilNode(assignValue_);
  return assignValue_ != nullptr;
  //  || (n != nullptr && n->isUserSpecified());
}

bool Type::isNilable(Type* t) { return t->isNilable(); }
bool Type::isRef(Type* t) { return t->isRef(); }
bool Type::isNilType(Type* t) {
  auto pt = toPrimitiveType(t);
  return pt != nullptr && pt->primitiveType() == PrimitiveTypeEnum::NIL;
}
bool Type::isUnknownType(Type* t) {
  auto pt = toPrimitiveType(t);
  return pt != nullptr && pt->primitiveType() == PrimitiveTypeEnum::UNKNOWN;
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

bool Type::isSameType(NodeList* t1, NodeList* t2) {
  if(t1->size() == t2->size()) {
    for(size_t i = 0; i < t1->size(); i++) {
      auto pt1 = toTypeNode((*t1)[i]);
      auto pt2 = toTypeNode((*t2)[i]);
      if(pt1 != nullptr && pt2 != nullptr) {
        if(!isSameType(pt1, pt2)) return false;
      } else throw ASTException("Expected a Type");
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

std::string Type::getTypeString(Type* t) {
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

ClassDefinition*
ClassDefinition::buildClass(ClassType* classType, NodeList* defs) {

  auto c = new ClassDefinition(classType);
  for(auto def : *defs) {
    if(isDefExpressionNode(def)) {
      c->variables_->addBack(toDefExpressionNode(def));
    } else if(isFunctionDefinitionNode(def)) {
      c->functions_->addBack(toFunctionDefinitionNode(def));
    } else if(isInitDefinitionNode(def)) {
      auto init = toInitDefinitionNode(def);
      if(init->isDeinit()) c->deinitializer_ = init;
      else c->initializers_->addBack(init);
    }
  }
  if(c->deinitializer_ == nullptr) {
    c->deinitializer_ = new InitDefinition(new Scope(new NodeList()));
  }
  return c;
}

bool IfStatement::hasElseBody() { return elseBody_ != nullptr; }
bool IfStatement::hasPlainElse() {
  return hasElseBody() && isScopeNode(elseBody_);
}
bool IfStatement::hasElseIf() {
  return hasElseBody() && isIfStatementNode(elseBody_);
}
Scope* IfStatement::plainElseBody() { return toScopeNode(elseBody_); }
IfStatement* IfStatement::elseIfBody() { return toIfStatementNode(elseBody_); }

char codeForEscapedChar(char c) {
  if(c == 'n') return '\n';
  if(c == 't') return '\t';
  if(c == '\\') return '\\';
  return c;
}

std::string StringExpression::escapedValue() {
  std::string v = str;
  // size_t pos = 0;
  while(true) {
    std::cout << "hewp\n";
    size_t pos = v.find('\\');
    if(pos == std::string::npos) break;
    char replacementChar = codeForEscapedChar(v[pos + 1]);
    std::string replacement;
    replacement += replacementChar;
    v.replace(pos, 2, replacement);
  }
  return v;
}

#define ast_node_def(type)                                                     \
  void type::accept(visitor::ASTVisitor* ast) { ast->visit(this); }
#include "ast-node.inc"

} // namespace ast
