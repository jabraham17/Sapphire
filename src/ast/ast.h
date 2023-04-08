#ifndef SAPPHIRE_AST_AST_H_
#define SAPPHIRE_AST_AST_H_

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace ast {
class ASTNode {
private:
  bool isRoot;

public:
  ASTNode() = default;
  ASTNode(bool isRoot) : isRoot(isRoot) {}
  virtual ~ASTNode() = default;
};

// FT is tempated
#define ast_node_list(F, FT)                                                   \
  F(ASTNode)                                                                   \
  FT(NodeList)                                                                 \
  F(Definition)                                                                \
  F(Parameter)                                                                 \
  F(FunctionPrototype)                                                         \
  F(FunctionDefinition)                                                        \
  F(ExternDefinition)                                                          \
  F(InitDefinition)                                                            \
  F(Statement)                                                                 \
  F(Scope)                                                                     \
  F(Expression)                                                                \
  F(DefExpression)                                                             \
  F(ClassDefinition)                                                           \
  F(OperatorDefinition)                                                        \
  F(Type)                                                                      \
  F(PrimitiveType)                                                             \
  F(ArrayType)                                                                 \
  F(TupleType)                                                                 \
  F(CallableType)                                                              \
  F(ClassType)                                                                 \
  F(IfStatement)                                                               \
  F(WhileStatement)                                                            \
  F(ForStatement)                                                              \
  F(Closure)                                                                   \
  F(Operator)                                                                  \
  F(CallExpression)                                                            \
  F(UseExpression)                                                             \
  FT(NumberExpression)                                                         \
  F(StringExpression)                                                          \
  F(Nil)

struct ASTException : public std::runtime_error {
  ASTException() : std::runtime_error("Unknown AST Exception") {}
  ASTException(std::string message)
      : std::runtime_error("AST Exception: " + message) {}
};

template <typename T = ASTNode> class NodeList : public ASTNode {
  static_assert(
      std::is_convertible_v<T*, ASTNode*>,
      "Type must derive ASTNode");
  friend ASTNode;

private:
  std::vector<T*> elms;

public:
  NodeList() = default;
  virtual ~NodeList() = default;
  void addElement(T* elm) { elms.push_back(elm); }
  auto getElement(size_t idx) { return elms[idx]; }
  auto getElement(size_t idx) const { return elms[idx]; }
  auto size() { return elms.size(); }
  auto size() const { return elms.size(); }
  auto begin() { return elms.begin(); }
  auto begin() const { return elms.begin(); }
  auto end() { return elms.end(); }
  auto end() const { return elms.end(); }
  auto operator[](size_t idx) { return getElement(idx); }
  auto operator[](size_t idx) const { return getElement(idx); }
};

class Definition : public ASTNode {
public:
  Definition() = default;
  virtual ~Definition() = default;
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
  Symbol* symbol;
  Type* type_;

public:
  Parameter(Symbol* symbol, Type* type) : symbol(symbol), type_(type) {}
  virtual ~Parameter() = default;
  Type* type() { return type_; }
};

class FunctionPrototype : public ASTNode {
private:
  std::optional<std::string> namespaceName;
  std::string name;
  NodeList<Parameter>* parameters;
  Type* returnType;

public:
  FunctionPrototype(
      const char* name,
      NodeList<Parameter>* parameters,
      Type* returnType)
      : name(name), parameters(parameters), returnType(returnType) {}
  FunctionPrototype(
      const char* namespaceName,
      const char* name,
      NodeList<Parameter>* parameters,
      Type* returnType)
      : FunctionPrototype(name, parameters, returnType) {
    setNamespace(namespaceName);
  }
  virtual ~FunctionPrototype() = default;
  void setNamespace(const char* namespaceName) {
    this->namespaceName = namespaceName;
  }
};

class Statement;
class Scope;

class FunctionDefinition : public Definition {
private:
  FunctionPrototype* functionPrototype;
  Scope* body;

public:
  FunctionDefinition(FunctionPrototype* functionPrototype, Scope* body)
      : functionPrototype(functionPrototype), body(body) {}
  virtual ~FunctionDefinition() = default;
};
class ExternDefinition : public Definition {
private:
  FunctionPrototype* functionPrototype;

public:
  ExternDefinition(FunctionPrototype* functionPrototype)
      : functionPrototype(functionPrototype) {}
  virtual ~ExternDefinition() = default;
};

class InitDefinition : public Definition {
private:
  NodeList<Parameter>* parameters;
  Scope* body;
  bool isInit;

public:
  InitDefinition(NodeList<Parameter>* parameters, Scope* body)
      : parameters(parameters), body(body), isInit(true) {}
  InitDefinition(Scope* body)
      : parameters(new NodeList<Parameter>), body(body), isInit(false) {}
  virtual ~InitDefinition() = default;
  bool isDeinit() { return !isInit; }
};

class Statement : public ASTNode {
public:
  Statement() = default;
  virtual ~Statement() = default;
};
class Scope : public Statement {
  NodeList<Statement>* statements;

public:
  Scope(NodeList<Statement>* statements) : statements(statements){};
  virtual ~Scope() = default;
};
class Expression : public Statement {
  Type* type_;

public:
  Expression(Type* type) : type_(type) {}
  Expression(PrimitiveTypeEnum type);
  virtual ~Expression() = default;
  virtual bool isLValue() { return false; }
  virtual Type* type() { return type_; }
};

class DefExpression : public Expression {
  Symbol* symbol;
  Expression* assignValue;

public:
  DefExpression(Type* type, Symbol* symbol, Expression* assignValue);
  DefExpression(Type* type, Symbol* symbol);
  DefExpression(Symbol* symbol, Expression* assignValue);
  DefExpression(Symbol* symbol);
  virtual ~DefExpression() = default;
  virtual void setValue(Expression* value) { assignValue = value; }
};
class ClassDefinition : public Definition {
  Symbol* symbol;
  NodeList<DefExpression>* variables;
  NodeList<FunctionDefinition>* functions;
  NodeList<InitDefinition>* initializers;
  InitDefinition* deinitializer;

public:
  ClassDefinition(Symbol* symbol)
      : symbol(symbol), variables(new NodeList<DefExpression>()),
        functions(new NodeList<FunctionDefinition>()),
        initializers(new NodeList<InitDefinition>()), deinitializer(nullptr) {}
  virtual ~ClassDefinition() = default;

  static ClassDefinition* buildClass(Symbol* symbol, NodeList<ASTNode>* defs);
};
class Operator;
class OperatorDefinition : public Definition {
  Operator* op;
  NodeList<Parameter>* parameters;
  Scope* body;

public:
  OperatorDefinition(Operator* op, NodeList<Parameter>* parameters, Scope* body)
      : op(op), parameters(parameters), body(body) {}
  virtual ~OperatorDefinition() = default;
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

public:
  Type(bool isRef, bool isNilable) : isRef_(isRef), isNilable_(isNilable) {}
  virtual ~Type() = default;

  virtual void setRef(bool isRef = true) { this->isRef_ = isRef; }
  virtual void setNilable(bool isNilable = true) {
    this->isNilable_ = isNilable;
  }
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

  static bool isSameType(NodeList<Type>* t1, NodeList<Type>* t2);

  static Type* getNilType();
  static Type* getUnknownType();
};
static NodeList<Type>* getParameterTypes(const NodeList<Parameter>& params) {
  auto types = new NodeList<Type>();
  for(auto p : params) {
    types->addElement(p->type());
  }
  return std::move(types);
}
class PrimitiveType : public Type {
  friend Type;

private:
  PrimitiveTypeEnum primType;

public:
  PrimitiveType(PrimitiveTypeEnum type) : PrimitiveType(type, false, false) {}
  PrimitiveType(PrimitiveTypeEnum type, bool isRef, bool isNilable)
      : Type(isRef, isNilable), primType(type) {}
  virtual ~PrimitiveType() = default;
};
class ArrayType : public Type {
  friend Type;

private:
  Type* arrayType;

public:
  ArrayType(Type* arrayType) : ArrayType(arrayType, false, false) {}
  ArrayType(Type* arrayType, bool isRef, bool isNilable)
      : Type(isRef, isNilable), arrayType(arrayType) {}
  virtual ~ArrayType() = default;
};

class TupleType : public Type {
  friend Type;

private:
  NodeList<Type>* tupleTypes;

public:
  TupleType(NodeList<Type>* tupleTypes) : TupleType(tupleTypes, false, false) {}
  TupleType(NodeList<Type>* tupleTypes, bool isRef, bool isNilable)
      : Type(isRef, isNilable), tupleTypes(tupleTypes) {}
  virtual ~TupleType() = default;
};
class Closure;
class CallableType : public Type {
  friend Type;
  friend Closure;

private:
  NodeList<Type>* parameterTypes;
  Type* returnType;

public:
  CallableType(NodeList<Type>* parameterTypes, Type* returnType)
      : CallableType(parameterTypes, returnType, false, false) {}
  CallableType(
      NodeList<Type>* parameterTypes,
      Type* returnType,
      bool isRef,
      bool isNilable)
      : Type(isRef, isNilable), parameterTypes(parameterTypes),
        returnType(returnType) {}
  virtual ~CallableType() = default;
};
class ClassType : public Type {
  friend Type;

private:
  Symbol* className;

public:
  ClassType(Symbol* className) : ClassType(className, false, false) {}
  ClassType(Symbol* className, bool isRef, bool isNilable)
      : Type(isRef, isNilable), className(className) {}

  virtual ~ClassType() = default;
};

class IfStatement : public Statement {
  Expression* expr;
  Scope* ifBody;
  // can be nullptr
  Statement* elseBody; // either Scope or IfStatement
public:
  IfStatement(Expression* expr, Scope* ifBody, Statement* elseBody)
      : expr(expr), ifBody(ifBody), elseBody(elseBody) {}
  IfStatement(Expression* expr, Scope* ifBody)
      : IfStatement(expr, ifBody, nullptr) {}
  virtual ~IfStatement() = default;
};
class WhileStatement : public Statement {
  Expression* expr;
  Scope* body;

public:
  WhileStatement(Expression* expr, Scope* body) : expr(expr), body(body) {}
  virtual ~WhileStatement() = default;
};
class ForStatement : public Statement {
  DefExpression* var;
  Expression* expr;
  Scope* body;

public:
  ForStatement(DefExpression* var, Expression* expr, Scope* body)
      : var(var), expr(expr), body(body) {}
  virtual ~ForStatement() = default;
};
class Symbol;

class Closure : public Expression {
  NodeList<Parameter>* parameters;
  Scope* body;

public:
  Closure(CallableType* type, NodeList<Parameter>* parameters, Scope* body)
      : Expression(type), parameters(parameters), body(body) {}
  virtual ~Closure() = default;
  static CallableType* determineClosureType(
      Type* type_specifier,
      const NodeList<Parameter>& parameters,
      Type* returnType) {
    // if no callable type (PrimitiveTypeEnum::NIL) then construct from params
    // and returnType if callable type, number and type of paramers must match
    // exactly and return type must match ( if specified, otherwise we can infer
    // it)
    if(Type::isNilType(type_specifier))
      return new CallableType(getParameterTypes(parameters), returnType);
    CallableType* callableType = Type::toCallableType(type_specifier);

    if(callableType == nullptr)
      throw ASTException("Cannot define a callable with a non-callable type");

    auto callableParamTypes = callableType->parameterTypes;
    auto paramTypes = getParameterTypes(parameters);
    bool paramTypesMatch = Type::isSameType(callableParamTypes, paramTypes);

    if(paramTypesMatch &&
       (Type::isNilType(returnType) ||
        Type::isSameType(returnType, callableType->returnType))) {
      return callableType;
    } else {
      throw ASTException("Callable type does not match");
    }
  }
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
class Operator : public ASTNode {
  OperatorType opType;

public:
  Operator(OperatorType opType) : opType(opType){};
  virtual ~Operator() = default;
};

class CallExpression : public Expression {
  Operator* op;
  NodeList<ASTNode>* operands;

public:
  template <typename... Args>
  CallExpression(OperatorType op, Args... operands)
      : Expression(Type::getUnknownType()), op(new Operator(op)),
        operands(new NodeList<ASTNode>()) {
    initOperands(std::forward<Args>(operands)...);
  }
  template <typename... Args>
  CallExpression(Operator* op, Args... operands)
      : Expression(Type::getUnknownType()), op(op),
        operands(new NodeList<ASTNode>()) {
    initOperands(std::forward<Args>(operands)...);
  }
  virtual ~CallExpression() = default;

private:
  template <typename Arg> void initOperands(Arg operand) {
    this->operands->addElement(operand);
  }
  template <typename Arg, typename... Args>
  void initOperands(Arg operand, Args... operands) {
    initOperands(std::forward<Arg>(operand));
    initOperands(std::forward<Args>(operands)...);
  }
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
};

class UseExpression : public Expression {
private:
  Symbol* symbol;

public:
  UseExpression(Symbol* symbol) : Expression(symbol->type()), symbol(symbol) {}
  virtual ~UseExpression() = default;
};

template <typename ValueType = long long>
class NumberExpression : public Expression {
private:
  ValueType value;

  template <typename VT>
  static constexpr PrimitiveTypeEnum typeBasedOnValueType() {
    if constexpr(std::is_integral_v<VT> && std::is_signed_v<VT>)
      return PrimitiveTypeEnum::INT;
    if constexpr(std::is_integral_v<VT> && std::is_unsigned_v<VT>)
      return PrimitiveTypeEnum::UINT;
    if constexpr(std::is_integral_v<VT> && std::is_floating_point_v<VT>)
      return PrimitiveTypeEnum::REAL;
    return PrimitiveTypeEnum::UNKNOWN;
  }

public:
  NumberExpression(ValueType value)
      : Expression(typeBasedOnValueType<ValueType>()), value(value) {}
  virtual ~NumberExpression() = default;
};

class StringExpression : public Expression {
  std::string str;

public:
  StringExpression(const std::string str)
      : Expression(PrimitiveTypeEnum::STRING), str(str) {}
  virtual ~StringExpression() = default;
};

class Nil : public Expression {
public:
  Nil() : Expression(Type::getNilType()) {}
  virtual ~Nil() = default;
};

#define ast_funcs(type)                                                        \
  static type* to##type##Node(ASTNode* a) { return dynamic_cast<type*>(a); }   \
  static bool is##type##Node(ASTNode* a) {                                     \
    return to##type##Node(a) != nullptr;                                       \
  }
#define ast_funcs_templated(type)                                              \
  template <typename T> static type<T>* to##type##Node(ASTNode* a) {           \
    return dynamic_cast<type<T>*>(a);                                          \
  }                                                                            \
  template <typename T> static bool is##type##Node(ASTNode* a) {               \
    return to##type##Node<T>(a) != nullptr;                                    \
  }
ast_node_list(ast_funcs, ast_funcs_templated)
#undef ast_funcs
#undef ast_funcs_templated

} // namespace ast

#endif
