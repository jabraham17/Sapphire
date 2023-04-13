#ifndef SAPPHIRE_AST_AST_H_
#define SAPPHIRE_AST_AST_H_

#include <cassert>
#include <deque>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace ast {
namespace visitor {
class ASTVisitor;
}
class ASTNode {
private:
  long lineNumber;

public:
  ASTNode() : lineNumber(-1){};
  virtual ~ASTNode() = default;
  // virtual  toString() { return "ast"; }
  virtual void accept(visitor::ASTVisitor* ast) = 0;
  long line() { return lineNumber; }
  void setLine(long line) { this->lineNumber = line; }
};

struct ASTException : public std::runtime_error {
  ASTException() : ASTException("Unknown AST Exception") {}
  ASTException(std::string message) : std::runtime_error(message) {}
};

class NodeList : public ASTNode {
private:
  std::deque<ASTNode*> elms;

public:
  NodeList() = default;
  virtual ~NodeList() = default;
  // virtual std::string toString() override { return ""; }
  virtual void accept(visitor::ASTVisitor* ast) override;

  void addFront(ASTNode* elm) { elms.push_front(elm); }
  void addBack(ASTNode* elm) { elms.push_back(elm); }
  auto get(size_t idx) { return elms[idx]; }
  auto get(size_t idx) const { return elms[idx]; }
  auto size() { return elms.size(); }
  auto size() const { return elms.size(); }
  auto begin() { return elms.begin(); }
  auto begin() const { return elms.begin(); }
  auto end() { return elms.end(); }
  auto end() const { return elms.end(); }
  auto operator[](size_t idx) { return get(idx); }
  auto operator[](size_t idx) const { return get(idx); }
};

class Symbol;
enum class PrimitiveTypeEnum {
  INT,
  UINT,
  REAL,
  STRING,
  BOOL,
  BYTE,
  NIL,
  UNKNOWN
};
class Type;

class Parameter : public ASTNode {
private:
  Symbol* symbol_;

public:
  Parameter(long lineNumber, Symbol* symbol, Type* type)
      : Parameter(symbol, type) {
    setLine(lineNumber);
  }
  Parameter(Symbol* symbol, Type* type);
  virtual ~Parameter() = default;
  Symbol* symbol() { return symbol_; }
  // virtual std::string toString() override;
  virtual void accept(visitor::ASTVisitor* ast) override;
};

class FunctionPrototype : public ASTNode {
private:
  std::optional<std::string> namespaceName_;
  std::string name_;
  NodeList* parameters_;
  Type* returnType_;
  bool shouldMangle_;
  Symbol* funcSymbol_;

public:
  FunctionPrototype(
      long lineNumber,
      const char* name,
      NodeList* parameters,
      Type* returnType)
      : FunctionPrototype(name, parameters, returnType) {
    setLine(lineNumber);
  }
  FunctionPrototype(const char* name, NodeList* parameters, Type* returnType);
  FunctionPrototype(
      long lineNumber,
      const char* namespaceName,
      const char* name,
      NodeList* parameters,
      Type* returnType)
      : FunctionPrototype(namespaceName, name, parameters, returnType) {
    setLine(lineNumber);
  }
  FunctionPrototype(
      const char* namespaceName,
      const char* name,
      NodeList* parameters,
      Type* returnType);
  virtual ~FunctionPrototype() = default;
  // virtual std::string toString() override;
  virtual void accept(visitor::ASTVisitor* ast) override;

  std::string name() {
    std::string s;
    if(namespaceName_.has_value()) s += *namespaceName_ + ".";
    s += name_;
    return s;
  }
  void setMangled(bool shouldMangle = true);
  std::string mangledNamed() {
    std::string s;
    if(namespaceName_.has_value()) s += *namespaceName_ + "_";
    s += name_;
    if(shouldMangle_) s = "spp_" + s;
    return s;
  }
  NodeList* parameters() { return parameters_; }
  Type* returnType() { return returnType_; }
  Symbol* functionSymbol() { return funcSymbol_; }

  void setNamespace(const char* namespaceName);
};

class Statement;
class Scope;

class FunctionDefinition : public ASTNode {
private:
  FunctionPrototype* functionPrototype_;
  Scope* body_;

public:
  FunctionDefinition(
      long lineNumber,
      FunctionPrototype* functionPrototype,
      Scope* body)
      : FunctionDefinition(functionPrototype, body) {
    setLine(lineNumber);
  }
  FunctionDefinition(FunctionPrototype* functionPrototype, Scope* body)
      : functionPrototype_(functionPrototype), body_(body) {}
  virtual ~FunctionDefinition() = default;
  // virtual std::string toString() override;
  virtual void accept(visitor::ASTVisitor* ast) override;

  FunctionPrototype* functionPrototype() { return functionPrototype_; }
  Scope* body() { return body_; }
};
class ExternDefinition : public ASTNode {
private:
  FunctionPrototype* functionPrototype_;

public:
  ExternDefinition(long lineNumber, FunctionPrototype* functionPrototype)
      : ExternDefinition(functionPrototype) {
    setLine(lineNumber);
  }
  ExternDefinition(FunctionPrototype* functionPrototype)
      : functionPrototype_(functionPrototype) {}
  virtual ~ExternDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  FunctionPrototype* functionPrototype() { return functionPrototype_; }
};

class InitDefinition : public ASTNode {
private:
  NodeList* parameters_;
  Scope* body_;
  bool isInit_;

public:
  InitDefinition(long lineNumber, NodeList* parameters, Scope* body)
      : InitDefinition(parameters, body) {
    setLine(lineNumber);
  }
  InitDefinition(NodeList* parameters, Scope* body)
      : parameters_(parameters), body_(body), isInit_(true) {}
  InitDefinition(long lineNumber, Scope* body) : InitDefinition(body) {
    setLine(lineNumber);
  }
  InitDefinition(Scope* body)
      : parameters_(nullptr), body_(body), isInit_(false) {}
  virtual ~InitDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  bool isDeinit() { return !isInit_; }
  bool isInit() { return isInit_; }
  NodeList* parameters() { return parameters_; }
  Scope* body() { return body_; }
};

class Statement : public ASTNode {

public:
  Statement() = default;
  virtual ~Statement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
};

class Scope : public Statement {
  NodeList* statements_;

public:
  Scope(long lineNumber, NodeList* statements) : Scope(statements) {
    setLine(lineNumber);
  };
  Scope(NodeList* statements) : statements_(statements){};
  virtual ~Scope() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  NodeList* statements() { return statements_; }
};
class Expression : public ASTNode {
private:
  bool isLHSOfAssign_ = false;

public:
  Expression() {}
  virtual ~Expression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual Type* type() = 0;
  virtual void setType(Type* type) = 0;
  void setLHSOfAssign(bool isLHSOfAssign = true) {
    isLHSOfAssign_ = isLHSOfAssign;
  }
  bool isLHSOfAssign() { return isLHSOfAssign_; }
};

class ExpressionStatement : public Statement {
  Expression* expression_;

public:
  ExpressionStatement(long lineNumber, Expression* expr)
      : ExpressionStatement(expr) {
    setLine(lineNumber);
  }
  ExpressionStatement(Expression* expr) : expression_(expr) {}
  virtual ~ExpressionStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  Expression* expression() { return expression_; }
};

class DefExpression : public Expression {
  Symbol* symbol_;
  Expression* assignValue_;

public:
  DefExpression(
      long lineNumber,
      Type* type,
      Symbol* symbol,
      Expression* assignValue)
      : DefExpression(type, symbol, assignValue) {
    setLine(lineNumber);
  }
  DefExpression(Type* type, Symbol* symbol, Expression* assignValue);
  DefExpression(long lineNumber, Type* type, Symbol* symbol)
      : DefExpression(type, symbol) {
    setLine(lineNumber);
  }
  DefExpression(Type* type, Symbol* symbol);
  DefExpression(long lineNumber, Symbol* symbol, Expression* assignValue)
      : DefExpression(symbol, assignValue) {
    setLine(lineNumber);
  }
  DefExpression(Symbol* symbol, Expression* assignValue);
  DefExpression(long lineNumber, Symbol* symbol) : DefExpression(symbol) {
    setLine(lineNumber);
  }
  DefExpression(Symbol* symbol);
  virtual ~DefExpression() = default;
  virtual void setValue(Expression* value) { assignValue_ = value; }
  virtual void accept(visitor::ASTVisitor* ast) override;
  Symbol* symbol() { return symbol_; }
  virtual Type* type() override;
  virtual void setType(Type* type) override;
  bool hasInitialValue();
  Expression* assignValue() { return assignValue_; }
};
class ClassType;
class ClassDefinition : public ASTNode {
  ClassType* classType_;
  NodeList* variables_;
  NodeList* functions_;
  NodeList* initializers_;
  InitDefinition* deinitializer_;

public:
  ClassDefinition(long lineNumber, ClassType* classType)
      : ClassDefinition(classType) {
    setLine(lineNumber);
  }
  ClassDefinition(ClassType* classType)
      : classType_(classType), variables_(new NodeList()),
        functions_(new NodeList()), initializers_(new NodeList()),
        deinitializer_(nullptr) {}
  virtual ~ClassDefinition() = default;

  static ClassDefinition* buildClass(ClassType* classType, NodeList* defs);
  virtual void accept(visitor::ASTVisitor* ast) override;
  ClassType* classType() { return classType_; }
  NodeList* variables() { return variables_; }
  NodeList* functions() { return functions_; }
  NodeList* initializers() { return initializers_; }
  InitDefinition* deinitializer() { return deinitializer_; }
};
class Operator;
class OperatorDefinition : public ASTNode {
  Operator* op_;
  NodeList* parameters_;
  Scope* body_;

public:
  OperatorDefinition(
      long lineNumber,
      Operator* op,
      NodeList* parameters,
      Scope* body)
      : OperatorDefinition(op, parameters, body) {
    setLine(lineNumber);
  }
  OperatorDefinition(Operator* op, NodeList* parameters, Scope* body)
      : op_(op), parameters_(parameters), body_(body) {}
  virtual ~OperatorDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  Operator* op() { return op_; }
  NodeList* parameters() { return parameters_; }
  Scope* body() { return body_; }
};

class PrimitiveType;
class ArrayType;
class TupleType;
class CallableType;
class ClassType;
class Type : public ASTNode {
private:
  bool isRef_;
  bool isNilable_;
  bool isUserSpecified_;

public:
  Type(bool isRef, bool isNilable)
      : isRef_(isRef), isNilable_(isNilable), isUserSpecified_(false) {}
  virtual ~Type() = default;

  virtual void setRef(bool isRef = true) { this->isRef_ = isRef; }
  virtual void setNilable(bool isNilable = true) {
    this->isNilable_ = isNilable;
  }
  void setUserSpecified(bool isUserSpecified = true) {
    this->isUserSpecified_ = isUserSpecified;
  }
  bool isRef() { return isRef_; }
  bool isNilable() { return isNilable_; }
  bool isUserSpecified() { return isUserSpecified_; }

  static bool isNilable(Type* t);
  static bool isRef(Type* t);
  static bool isNilType(Type* t);
  static bool isUnknownType(Type* t);

  static bool isPrimitiveType(Type* t);
  static bool isArrayType(Type* t);
  static bool isTupleType(Type* t);
  static bool isCallableType(Type* t);
  static bool isClassType(Type* t);

  static PrimitiveType* toPrimitiveType(Type* t);
  static ArrayType* toArrayType(Type* t);
  static TupleType* toTupleType(Type* t);
  static CallableType* toCallableType(Type* t);
  static ClassType* toClassType(Type* t);

  // same type, just the base
  static bool isSameBaseType(Type* t1, Type* t2);
  // same type, including nilable and ref
  static bool isSameType(Type* t1, Type* t2);

  static bool isSameType(NodeList* t1, NodeList* t2);

  static Type* getNilType();
  static Type* getUnknownType();

  static std::string getTypeString(Type* t);

  virtual void accept(visitor::ASTVisitor* ast) override;
};

class PrimitiveType : public Type {

private:
  PrimitiveTypeEnum primType;

public:
  PrimitiveType(long lineNumber, PrimitiveTypeEnum type) : PrimitiveType(type) {
    setLine(lineNumber);
  }
  PrimitiveType(PrimitiveTypeEnum type) : PrimitiveType(type, false, false) {}
  PrimitiveType(
      long lineNumber,
      PrimitiveTypeEnum type,
      bool isRef,
      bool isNilable)
      : PrimitiveType(type, isRef, isNilable) {
    setLine(lineNumber);
  }
  PrimitiveType(PrimitiveTypeEnum type, bool isRef, bool isNilable)
      : Type(isRef, isNilable), primType(type) {}
  virtual ~PrimitiveType() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  PrimitiveTypeEnum primitiveType() { return primType; }
};
class ArrayType : public Type {

private:
  Type* arrayType;

public:
  ArrayType(long lineNumber, Type* arrayType) : ArrayType(arrayType) {
    setLine(lineNumber);
  }
  ArrayType(Type* arrayType) : ArrayType(arrayType, false, false) {}
  ArrayType(long lineNumber, Type* arrayType, bool isRef, bool isNilable)
      : ArrayType(arrayType, isRef, isNilable) {
    setLine(lineNumber);
  }
  ArrayType(Type* arrayType, bool isRef, bool isNilable)
      : Type(isRef, isNilable), arrayType(arrayType) {}
  virtual ~ArrayType() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  Type* elementType() { return arrayType; }
};

class TupleType : public Type {

private:
  NodeList* tupleTypes;

public:
  TupleType(long lineNumber, NodeList* tupleTypes) : TupleType(tupleTypes) {
    setLine(lineNumber);
  }
  TupleType(NodeList* tupleTypes) : TupleType(tupleTypes, false, false) {}
  TupleType(long lineNumber, NodeList* tupleTypes, bool isRef, bool isNilable)
      : TupleType(tupleTypes, isRef, isNilable) {
    setLine(lineNumber);
  }
  TupleType(NodeList* tupleTypes, bool isRef, bool isNilable)
      : Type(isRef, isNilable), tupleTypes(tupleTypes) {}
  virtual ~TupleType() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  NodeList* elementTypes() { return tupleTypes; }
};
class Closure;
class CallableType : public Type {

private:
  NodeList* parameterTypes_;
  Type* returnType_;

public:
  CallableType(long lineNumber, NodeList* parameterTypes, Type* returnType)
      : CallableType(parameterTypes, returnType) {
    setLine(lineNumber);
  }
  CallableType(NodeList* parameterTypes, Type* returnType)
      : CallableType(parameterTypes, returnType, false, false) {}
  CallableType(
      long lineNumber,
      NodeList* parameterTypes,
      Type* returnType,
      bool isRef,
      bool isNilable)
      : CallableType(parameterTypes, returnType, isRef, isNilable) {
    setLine(lineNumber);
  }
  CallableType(
      NodeList* parameterTypes,
      Type* returnType,
      bool isRef,
      bool isNilable)
      : Type(isRef, isNilable), parameterTypes_(parameterTypes),
        returnType_(returnType) {}
  virtual ~CallableType() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  NodeList* parameterTypes() { return parameterTypes_; }
  Type* returnType() { return returnType_; }
};
class ClassType : public Type {

private:
  std::string className_;

public:
  ClassType(long lineNumber, const char* className) : ClassType(className) {
    setLine(lineNumber);
  }
  ClassType(const char* className) : ClassType(className, false, false) {}
  ClassType(long lineNumber, const char* className, bool isRef, bool isNilable)
      : ClassType(className, isRef, isNilable) {
    setLine(lineNumber);
  }
  ClassType(const char* className, bool isRef, bool isNilable)
      : Type(isRef, isNilable), className_(className) {}

  virtual ~ClassType() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  std::string className() { return className_; }
};

class IfStatement : public Statement {
  Expression* expr_;
  Scope* ifBody_;
  // can be nullptr
  Statement* elseBody_; // either Scope or IfStatement
public:
  IfStatement(
      long lineNumber,
      Expression* expr,
      Scope* ifBody,
      Statement* elseBody)
      : IfStatement(expr, ifBody, elseBody) {
    setLine(lineNumber);
  }
  IfStatement(Expression* expr, Scope* ifBody, Statement* elseBody)
      : expr_(expr), ifBody_(ifBody), elseBody_(elseBody) {}
  IfStatement(long lineNumber, Expression* expr, Scope* ifBody)
      : IfStatement(expr, ifBody) {
    setLine(lineNumber);
  }
  IfStatement(Expression* expr, Scope* ifBody)
      : IfStatement(expr, ifBody, nullptr) {}
  virtual ~IfStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  Expression* expr() { return expr_; }
  Scope* ifBody() { return ifBody_; }
  bool hasElseBody();
  bool hasPlainElse();
  bool hasElseIf();
  Scope* plainElseBody();
  IfStatement* elseIfBody();
};
class WhileStatement : public Statement {
  Expression* expr_;
  Scope* body_;

public:
  WhileStatement(long lineNumber, Expression* expr, Scope* body)
      : WhileStatement(expr, body) {
    setLine(lineNumber);
  }
  WhileStatement(Expression* expr, Scope* body) : expr_(expr), body_(body) {}
  virtual ~WhileStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  Expression* expr() { return expr_; }
  Scope* body() { return body_; }
};
class ForStatement : public Statement {
  DefExpression* var_;
  Expression* expr_;
  Scope* body_;

public:
  ForStatement(
      long lineNumber,
      DefExpression* var,
      Expression* expr,
      Scope* body)
      : ForStatement(var, expr, body) {
    setLine(lineNumber);
  }
  ForStatement(DefExpression* var, Expression* expr, Scope* body)
      : var_(var), expr_(expr), body_(body) {}
  virtual ~ForStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  DefExpression* variable() { return var_; }
  Expression* expr() { return expr_; }
  Scope* body() { return body_; }
};
class ReturnStatement : public Statement {
  Expression* expr_;

public:
  ReturnStatement(long lineNumber, Expression* expr) : ReturnStatement(expr) {
    setLine(lineNumber);
  }
  ReturnStatement(Expression* expr) : expr_(expr) {}
  virtual ~ReturnStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  Expression* expression() { return expr_; }
};

class Closure : public Expression {
  CallableType* type_;
  NodeList* parameters_;
  Scope* body_;

public:
  Closure(
      long lineNumber,
      CallableType* type,
      NodeList* parameters,
      Scope* body)
      : Closure(type, parameters, body) {
    setLine(lineNumber);
  }
  Closure(CallableType* type, NodeList* parameters, Scope* body)
      : type_(type), parameters_(parameters), body_(body) {}
  virtual ~Closure() = default;
  virtual Type* type() override { return type_; }
  virtual void setType(Type* type) override {
    auto callableType = ast::Type::toCallableType(type);
    assert(callableType != nullptr && "type must be a callable type");
    this->type_ = callableType;
  }
  virtual void accept(visitor::ASTVisitor* ast) override;
  static CallableType* determineClosureType(
      Type* type_specifier,
      const NodeList& parameters,
      Type* returnType);

  NodeList* parameters() { return parameters_; }
  Scope* body() { return body_; }
};

enum class OperatorType {
  ASSIGNMENT,
  CONDITIONAL,
  NEW_CLASS,
  NEW_ARRAY,
  NEW_TUPLE,
  FUNCTION,
  FORCE_UNWRAP,
  OPTIONAL_UNWRAP,
  TAKE_REF,
  FIELD_ACCESS,
  SUBSCRIPT,
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
  LSHIFT,
  RSHIFT,
  MODULO,
  AND,
  OR,
  BW_AND,
  BW_OR,
  EQ,
  NEQ,
  LT,
  GT,
  LTEQ,
  GTEQ,
  BW_NEGATE,
  NEGATE,
  NOT
};
[[maybe_unused]] static std::string getOperatorTypeString(OperatorType ot) {
  switch(ot) {
    case OperatorType::ASSIGNMENT: return "=";
    case OperatorType::CONDITIONAL: return "?:";
    case OperatorType::NEW_CLASS: return "newClass";
    case OperatorType::NEW_ARRAY: return "newArray";
    case OperatorType::NEW_TUPLE: return "newTuple";
    case OperatorType::FUNCTION: return "call";
    case OperatorType::FORCE_UNWRAP: return "unwrap";
    case OperatorType::OPTIONAL_UNWRAP: return "unwrap?";
    case OperatorType::TAKE_REF: return "getRef";
    case OperatorType::FIELD_ACCESS: return ".";
    case OperatorType::SUBSCRIPT: return "[]";
    case OperatorType::PLUS: return "+";
    case OperatorType::MINUS: return "-";
    case OperatorType::MULTIPLY: return "*";
    case OperatorType::DIVIDE: return "/";
    case OperatorType::LSHIFT: return "<<";
    case OperatorType::RSHIFT: return ">>";
    case OperatorType::MODULO: return "&";
    case OperatorType::AND: return "&&";
    case OperatorType::OR: return "&";
    case OperatorType::BW_AND: return "&";
    case OperatorType::BW_OR: return "|";
    case OperatorType::EQ: return "==";
    case OperatorType::NEQ: return "!=";
    case OperatorType::LT: return "<";
    case OperatorType::GT: return ">";
    case OperatorType::LTEQ: return "<=";
    case OperatorType::GTEQ: return ">=";
    case OperatorType::BW_NEGATE: return "~";
    case OperatorType::NEGATE: return "-";
    case OperatorType::NOT: return "!";
  }
}
class Operator : public ASTNode {
  OperatorType opType_;

public:
  Operator(long lineNumber, OperatorType opType) : Operator(opType) {
    setLine(lineNumber);
  }
  Operator(OperatorType opType) : opType_(opType){};
  virtual ~Operator() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  OperatorType opType() { return opType_; }
};

class CallExpression : public Expression {
  Operator* op_;
  NodeList* operands_;
  Type* type_;

public:
  template <typename... Args>
  CallExpression(long lineNumber, OperatorType opType, Args... operands)
      : CallExpression(opType, operands...) {
    setLine(lineNumber);
  }
  template <typename... Args>
  CallExpression(OperatorType opType, Args... operands)
      : op_(new Operator(opType)), operands_(new NodeList()),
        type_(Type::getUnknownType()) {
    initOperands(std::forward<Args>(operands)...);
  }
  template <typename... Args>
  CallExpression(long lineNumber, Operator* op, Args... operands)
      : CallExpression(op, operands...) {
    setLine(lineNumber);
  }
  template <typename... Args>
  CallExpression(Operator* op, Args... operands)
      : op_(op), operands_(new NodeList()), type_(Type::getUnknownType()) {
    initOperands(std::forward<Args>(operands)...);
  }
  virtual ~CallExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual Type* type() override { return type_; }
  virtual void setType(Type* type) override { this->type_ = type; }

private:
  template <typename Arg> void initOperands(Arg operand) {
    // unwrap node lists past in
    if(auto nl = toNodeListNode(operand); nl != nullptr) {
      for(auto elm : *nl) {
        this->operands_->addBack(elm);
      }
    } else this->operands_->addBack(operand);
  }
  template <typename Arg, typename... Args>
  void initOperands(Arg operand, Args... operands) {
    initOperands(std::forward<Arg>(operand));
    initOperands(std::forward<Args>(operands)...);
  }

public:
  Operator* op() { return op_; }
  NodeList* operands() { return operands_; }
};
// separate symbol as a separate thing
// then add a DefExpressing thingy
class Symbol {
  std::string symbolName;
  Type* type_;

public:
  Symbol(const char* symbolName) : Symbol(symbolName, Type::getUnknownType()) {}
  Symbol(const char* symbolName, Type* type)
      : symbolName(symbolName), type_(type) {}
  virtual ~Symbol() = default;

  std::string name() { return symbolName; }
  void setName(const std::string& name) { symbolName = name; }
  Type* type() { return type_; }
  void setType(Type* t) { type_ = t; }
  std::string toString() {
    return symbolName + ": " + Type::getTypeString(type_);
  }
};

class UseExpression : public Expression {
private:
  Symbol* symbol_;

public:
  UseExpression(long lineNumber, Symbol* symbol) : UseExpression(symbol) {
    setLine(lineNumber);
  }
  UseExpression(Symbol* symbol) : symbol_(symbol) {}
  virtual ~UseExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  Symbol* symbol() { return symbol_; }
  virtual Type* type() override;
  virtual void setType(Type* type) override;
  void setSymbol(Symbol* symbol) { symbol_ = symbol; }
};

class IntExpression : public Expression {
private:
  long long value_;

public:
  IntExpression(long lineNumber, long long value) : IntExpression(value) {
    setLine(lineNumber);
  }
  IntExpression(long long value) : value_(value) {}
  virtual ~IntExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual Type* type() override {
    return new PrimitiveType(PrimitiveTypeEnum::INT);
  }
  virtual void setType(Type* type) override {
    assert(false && "cannot set the type of an int expression");
  }
  auto value() { return value_; }
};

class UIntExpression : public Expression {
private:
  unsigned long long value_;

public:
  UIntExpression(long lineNumber, unsigned long long value)
      : UIntExpression(value) {
    setLine(lineNumber);
  }
  UIntExpression(unsigned long long value) : value_(value) {}
  virtual ~UIntExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual Type* type() override {
    return new PrimitiveType(PrimitiveTypeEnum::UINT);
  }
  virtual void setType(Type* type) override {
    assert(false && "cannot set the type of an uint expression");
  }
  auto value() { return value_; }
};

class RealExpression : public Expression {
private:
  double value_;

public:
  RealExpression(long lineNumber, double value) : RealExpression(value) {
    setLine(lineNumber);
  }
  RealExpression(double value) : value_(value) {}
  virtual ~RealExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual Type* type() override {
    return new PrimitiveType(PrimitiveTypeEnum::REAL);
  }
  virtual void setType(Type* type) override {
    assert(false && "cannot set the type of a real expression");
  }
  auto value() { return value_; }
};

class StringExpression : public Expression {
private:
  std::string str;

public:
  StringExpression(long lineNumber, const std::string& value)
      : StringExpression(value) {
    setLine(lineNumber);
  }
  StringExpression(const std::string str) : str(str) {}
  virtual ~StringExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual Type* type() override {
    return new PrimitiveType(PrimitiveTypeEnum::STRING);
  }
  virtual void setType(Type* type) override {
    assert(false && "cannot set the type of a string expression");
  }
  std::string value() { return str; }
  //  return the string with all escapes resolved
  std::string escapedValue();
};

class Nil : public Expression {
  bool isUserSpecified_;

public:
  Nil(long lineNumber) : Nil() { setLine(lineNumber); }
  Nil() : Nil(false) {}
  Nil(long lineNumber, bool isUserSpecified) : Nil(isUserSpecified) {
    setLine(lineNumber);
  }
  Nil(bool isUserSpecified) : isUserSpecified_(isUserSpecified) {}
  virtual ~Nil() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual Type* type() override { return Type::getNilType(); }
  virtual void setType(Type* type) override {
    assert(false && "cannot set the type of nil");
  }
  bool isUserSpecified() { return isUserSpecified_; }
};

// defines the visitors
#define ast_node_def(type)                                                     \
  [[maybe_unused]] static type* to##type##Node(ASTNode* a) {                   \
    return dynamic_cast<type*>(a);                                             \
  }                                                                            \
  [[maybe_unused]] static bool is##type##Node(ASTNode* a) {                    \
    return to##type##Node(a) != nullptr;                                       \
  }
#include "ast-node.inc"

} // namespace ast

#endif
