#ifndef SAPPHIRE_AST_NODE_FUNCTIONPROTOTYPE_H_
#define SAPPHIRE_AST_NODE_FUNCTIONPROTOTYPE_H_
#include "ast/ast.h"
#include "ast/node/ASTNode.h"
namespace ast {
namespace node {
class FunctionPrototype : public ASTNode {
private:
  // which type this function belongs to
  Type* belongsTo_;
  NodeList* parameters_;
  symbol::FunctionSymbol* funcSymbol_;
  bool isMangled_;

  friend symbol::FunctionSymbol;

public:
  FunctionPrototype(
      long lineNumber,
      const char* name,
      NodeList* parameters,
      Type* returnType,
      bool isMangled = true,
      Type* belongsTo = nullptr)
      : FunctionPrototype(name, parameters, returnType, isMangled, belongsTo) {
    setLine(lineNumber);
  }
  FunctionPrototype(
      const char* name,
      NodeList* parameters,
      Type* returnType,
      bool isMangled = true,
      Type* belongsTo = nullptr);
  virtual ~FunctionPrototype() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  std::string name();
  NodeList* parameters() { return parameters_; }
  CallableType* type();
  symbol::FunctionSymbol* symbol() { return funcSymbol_; }
};
} // namespace node
} // namespace ast
#endif
