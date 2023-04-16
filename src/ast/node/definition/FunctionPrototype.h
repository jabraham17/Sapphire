#ifndef SAPPHIRE_AST_NODE_DEFINITION_FUNCTIONPROTOTYPE_H_
#define SAPPHIRE_AST_NODE_DEFINITION_FUNCTIONPROTOTYPE_H_
#include "ast/ast.h"

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
      long line,
      const char* name,
      NodeList* parameters,
      Type* returnType,
      bool isMangled,
      Type* belongsTo);
  FunctionPrototype(
      const char* name,
      NodeList* parameters,
      Type* returnType,
      bool isMangled,
      Type* belongsTo);

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
