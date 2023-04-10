#ifndef SAPPHIRE_AST_AST_H_
#define SAPPHIRE_AST_AST_H_

#include <deque>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace ast {
class ASTVisitor;
class ASTNode {
public:
  ASTNode() = default;
  virtual ~ASTNode() = default;
  // virtual  toString() { return "ast"; }
  virtual void accept(ASTVisitor* ast) = 0;
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
  virtual void accept(ASTVisitor* ast) override;

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
  Parameter(Symbol* symbol, Type* type);
  virtual ~Parameter() = default;
  Symbol* symbol() { return symbol_; }
  // virtual std::string toString() override;
  virtual void accept(ASTVisitor* ast) override;
};

class FunctionPrototype : public ASTNode {
private:
  std::optional<std::string> namespaceName_;
  std::string name_;
  NodeList* parameters_;
  Type* returnType_;

public:
  FunctionPrototype(const char* name, NodeList* parameters, Type* returnType)
      : name_(name), parameters_(parameters), returnType_(returnType) {}
  FunctionPrototype(
      const char* namespaceName,
      const char* name,
      NodeList* parameters,
      Type* returnType)
      : FunctionPrototype(name, parameters, returnType) {
    setNamespace(namespaceName);
  }
  virtual ~FunctionPrototype() = default;
  // virtual std::string toString() override;
  virtual void accept(ASTVisitor* ast) override;

  std::string name() {
    std::string s;
    if(namespaceName_.has_value()) s += *namespaceName_ + ".";
    s += name_;
    return s;
  }
  NodeList* parameters() { return parameters_; }
  Type* returnType() { return returnType_; }

  void setNamespace(const char* namespaceName) {
    this->namespaceName_ = namespaceName;
  }
};

class Statement;
class Scope;

class FunctionDefinition : public ASTNode {
private:
  FunctionPrototype* functionPrototype_;
  Scope* body_;

public:
  FunctionDefinition(FunctionPrototype* functionPrototype, Scope* body)
      : functionPrototype_(functionPrototype), body_(body) {}
  virtual ~FunctionDefinition() = default;
  // virtual std::string toString() override;
  virtual void accept(ASTVisitor* ast) override;

  FunctionPrototype* functionPrototype() { return functionPrototype_; }
  Scope* body() { return body_; }
};
class ExternDefinition : public ASTNode {
private:
  FunctionPrototype* functionPrototype_;

public:
  ExternDefinition(FunctionPrototype* functionPrototype)
      : functionPrototype_(functionPrototype) {}
  virtual ~ExternDefinition() = default;
  virtual void accept(ASTVisitor* ast) override;
  FunctionPrototype* functionPrototype() { return functionPrototype_; }
};

class InitDefinition : public ASTNode {
private:
  NodeList* parameters_;
  Scope* body_;
  bool isInit_;

public:
  InitDefinition(NodeList* parameters, Scope* body)
      : parameters_(parameters), body_(body), isInit_(true) {}
  InitDefinition(Scope* body)
      : parameters_(nullptr), body_(body), isInit_(false) {}
  virtual ~InitDefinition() = default;
  virtual void accept(ASTVisitor* ast) override;
  bool isDeinit() { return !isInit_; }
  bool isInit() { return isInit_; }
  NodeList* parameters() { return parameters_; }
  Scope* body() { return body_; }
};

class Statement : public ASTNode {

public:
  Statement() = default;
  virtual ~Statement() = default;
  virtual void accept(ASTVisitor* ast) override;
};

class Scope : public Statement {
  NodeList* statements_;

public:
  Scope(NodeList* statements) : statements_(statements){};
  virtual ~Scope() = default;
  virtual void accept(ASTVisitor* ast) override;
  NodeList* statements() { return statements_; }
};
class Expression : public ASTNode {
  Type* type_;

public:
  Expression(Type* type) : type_(type) {}
  Expression(PrimitiveTypeEnum type);
  virtual ~Expression() = default;
  virtual bool isLValue() { return false; }
  virtual Type* type() { return type_; }
  virtual void accept(ASTVisitor* ast) override;
};

class ExpressionStatement : public Statement {
  Expression* expression_;

public:
  ExpressionStatement(Expression* expr) : expression_(expr) {}
  virtual ~ExpressionStatement() = default;
  virtual void accept(ASTVisitor* ast) override;
  Expression* expression() { return expression_; }
};

class DefExpression : public Expression {
  Symbol* symbol_;
  Expression* assignValue_;

public:
  DefExpression(Type* type, Symbol* symbol, Expression* assignValue);
  DefExpression(Type* type, Symbol* symbol);
  DefExpression(Symbol* symbol, Expression* assignValue);
  DefExpression(Symbol* symbol);
  virtual ~DefExpression() = default;
  virtual void setValue(Expression* value) { assignValue_ = value; }
  virtual void accept(ASTVisitor* ast) override;
  Symbol* symbol() { return symbol_; }
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
  ClassDefinition(ClassType* classType)
      : classType_(classType), variables_(new NodeList()),
        functions_(new NodeList()), initializers_(new NodeList()),
        deinitializer_(nullptr) {}
  virtual ~ClassDefinition() = default;

  static ClassDefinition* buildClass(ClassType* classType, NodeList* defs);
  virtual void accept(ASTVisitor* ast) override;
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
  OperatorDefinition(Operator* op, NodeList* parameters, Scope* body)
      : op_(op), parameters_(parameters), body_(body) {}
  virtual ~OperatorDefinition() = default;
  virtual void accept(ASTVisitor* ast) override;

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

  virtual void accept(ASTVisitor* ast) override;
};

class PrimitiveType : public Type {

private:
  PrimitiveTypeEnum primType;

public:
  PrimitiveType(PrimitiveTypeEnum type) : PrimitiveType(type, false, false) {}
  PrimitiveType(PrimitiveTypeEnum type, bool isRef, bool isNilable)
      : Type(isRef, isNilable), primType(type) {}
  virtual ~PrimitiveType() = default;
  virtual void accept(ASTVisitor* ast) override;
  PrimitiveTypeEnum primitiveType() { return primType; }
};
class ArrayType : public Type {

private:
  Type* arrayType;

public:
  ArrayType(Type* arrayType) : ArrayType(arrayType, false, false) {}
  ArrayType(Type* arrayType, bool isRef, bool isNilable)
      : Type(isRef, isNilable), arrayType(arrayType) {}
  virtual ~ArrayType() = default;
  virtual void accept(ASTVisitor* ast) override;
  Type* elementType() { return arrayType; }
};

class TupleType : public Type {

private:
  NodeList* tupleTypes;

public:
  TupleType(NodeList* tupleTypes) : TupleType(tupleTypes, false, false) {}
  TupleType(NodeList* tupleTypes, bool isRef, bool isNilable)
      : Type(isRef, isNilable), tupleTypes(tupleTypes) {}
  virtual ~TupleType() = default;
  virtual void accept(ASTVisitor* ast) override;
  NodeList* elementTypes() { return tupleTypes; }
};
class Closure;
class CallableType : public Type {

private:
  NodeList* parameterTypes_;
  Type* returnType_;

public:
  CallableType(NodeList* parameterTypes, Type* returnType)
      : CallableType(parameterTypes, returnType, false, false) {}
  CallableType(
      NodeList* parameterTypes,
      Type* returnType,
      bool isRef,
      bool isNilable)
      : Type(isRef, isNilable), parameterTypes_(parameterTypes),
        returnType_(returnType) {}
  virtual ~CallableType() = default;
  virtual void accept(ASTVisitor* ast) override;

  NodeList* parameterTypes() { return parameterTypes_; }
  Type* returnType() { return returnType_; }
};
class ClassType : public Type {

private:
  std::string className_;

public:
  ClassType(const char* className) : ClassType(className, false, false) {}
  ClassType(const char* className, bool isRef, bool isNilable)
      : Type(isRef, isNilable), className_(className) {}

  virtual ~ClassType() = default;
  virtual void accept(ASTVisitor* ast) override;
  std::string className() { return className_; }
};

class IfStatement : public Statement {
  Expression* expr_;
  Scope* ifBody_;
  // can be nullptr
  Statement* elseBody_; // either Scope or IfStatement
public:
  IfStatement(Expression* expr, Scope* ifBody, Statement* elseBody)
      : expr_(expr), ifBody_(ifBody), elseBody_(elseBody) {}
  IfStatement(Expression* expr, Scope* ifBody)
      : IfStatement(expr, ifBody, nullptr) {}
  virtual ~IfStatement() = default;
  virtual void accept(ASTVisitor* ast) override;
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
  WhileStatement(Expression* expr, Scope* body) : expr_(expr), body_(body) {}
  virtual ~WhileStatement() = default;
  virtual void accept(ASTVisitor* ast) override;

  Expression* expr() { return expr_; }
  Scope* body() { return body_; }
};
class ForStatement : public Statement {
  DefExpression* var_;
  Expression* expr_;
  Scope* body_;

public:
  ForStatement(DefExpression* var, Expression* expr, Scope* body)
      : var_(var), expr_(expr), body_(body) {}
  virtual ~ForStatement() = default;
  virtual void accept(ASTVisitor* ast) override;
  DefExpression* variable() { return var_; }
  Expression* expr() { return expr_; }
  Scope* body() { return body_; }
};
class ReturnStatement : public Statement {
  Expression* expr_;

public:
  ReturnStatement(Expression* expr) : expr_(expr) {}
  virtual ~ReturnStatement() = default;
  virtual void accept(ASTVisitor* ast) override;
  Expression* expression() { return expr_; }
};

class Closure : public Expression {
  NodeList* parameters_;
  Scope* body_;

public:
  Closure(CallableType* type, NodeList* parameters, Scope* body)
      : Expression(type), parameters_(parameters), body_(body) {}
  virtual ~Closure() = default;
  virtual void accept(ASTVisitor* ast) override;
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
  Operator(OperatorType opType) : opType_(opType){};
  virtual ~Operator() = default;
  virtual void accept(ASTVisitor* ast) override;
  OperatorType opType() { return opType_; }
};

class CallExpression : public Expression {
  Operator* op_;
  NodeList* operands_;

public:
  template <typename... Args>
  CallExpression(OperatorType opType, Args... operands)
      : Expression(Type::getUnknownType()), op_(new Operator(opType)),
        operands_(new NodeList()) {
    initOperands(std::forward<Args>(operands)...);
  }
  template <typename... Args>
  CallExpression(Operator* op, Args... operands)
      : Expression(Type::getUnknownType()), op_(op), operands_(new NodeList()) {
    initOperands(std::forward<Args>(operands)...);
  }
  virtual ~CallExpression() = default;
  virtual void accept(ASTVisitor* ast) override;

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
  UseExpression(Symbol* symbol) : Expression(symbol->type()), symbol_(symbol) {}
  virtual ~UseExpression() = default;
  virtual void accept(ASTVisitor* ast) override;
  Symbol* symbol() { return symbol_; }
};

class IntExpression : public Expression {
private:
  long long value_;

public:
  IntExpression(long long value)
      : Expression(PrimitiveTypeEnum::INT), value_(value) {}
  virtual ~IntExpression() = default;
  virtual void accept(ASTVisitor* ast) override;
  auto value() { return value_; }
};

class UIntExpression : public Expression {
private:
  unsigned long long value_;

public:
  UIntExpression(long long value)
      : Expression(PrimitiveTypeEnum::INT), value_(value) {}
  virtual ~UIntExpression() = default;
  virtual void accept(ASTVisitor* ast) override;
  auto value() { return value_; }
};

class RealExpression : public Expression {
private:
  double value_;

public:
  RealExpression(double value)
      : Expression(PrimitiveTypeEnum::INT), value_(value) {}
  virtual ~RealExpression() = default;
  virtual void accept(ASTVisitor* ast) override;
  auto value() { return value_; }
};

class StringExpression : public Expression {
  std::string str;

public:
  StringExpression(const std::string str)
      : Expression(PrimitiveTypeEnum::STRING), str(str) {}
  virtual ~StringExpression() = default;
  virtual void accept(ASTVisitor* ast) override;
  auto value() { return str; }
};

class Nil : public Expression {
  bool isUserSpecified_;

public:
  Nil() : Nil(false) {}
  Nil(bool isUserSpecified)
      : Expression(Type::getNilType()), isUserSpecified_(isUserSpecified) {}
  virtual ~Nil() = default;
  virtual void accept(ASTVisitor* ast) override;
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
