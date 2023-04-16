#ifndef SAPPHIRE_AST_NODE_DEFINITION_FUNCTIONPROTOTYPE_H_
#define SAPPHIRE_AST_NODE_DEFINITION_FUNCTIONPROTOTYPE_H_
#include "ast/ast.h"

// TODO: cahnge belongsTo from a default into multiple constructors
// TODO: curioosuly recuovccuong template pattern to forward constructors
// TODO: refactor initOperands to use fold expressions
#include "ast/node/type/nodes.h"

#include <string>

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
      Type* belongsTo = ast::node::Type::getUntypedType());
  FunctionPrototype(
      const char* name,
      NodeList* parameters,
      Type* returnType,
      bool isMangled = true,
      Type* belongsTo = ast::node::Type::getUntypedType());
  // TODO: changer belongs to to default to
  virtual ~FunctionPrototype() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  std::string name();
  NodeList* parameters();
  CallableType* type();
  symbol::FunctionSymbol* symbol();
};
} // namespace node
} // namespace ast
#endif
