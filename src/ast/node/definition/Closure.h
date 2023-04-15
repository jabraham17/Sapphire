#ifndef SAPPHIRE_AST_NODE_CLOSURE_H_
#define SAPPHIRE_AST_NODE_CLOSURE_H_
#include "ast/ast.h"
#include "ast/node/Expression.h"
namespace ast {

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
} // namespace node
} // namespace ast
#endif
