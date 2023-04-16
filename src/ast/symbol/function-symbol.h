#ifndef SAPPHIRE_AST_SYMBOL_FUNCTION_SYMBOL_H_
#define SAPPHIRE_AST_SYMBOL_FUNCTION_SYMBOL_H_

#include "symbol.h"

#include "ast/ast.h"

#include <string>

namespace ast {

namespace symbol {

class FunctionSymbol : public Symbol {
  node::FunctionPrototype* prototype_;

public:
  FunctionSymbol(
      node::FunctionPrototype* prototype,
      const std::string& symbolName,
      node::Type* type);
  FunctionSymbol(
      node::FunctionPrototype* prototype,
      const char* symbolName,
      node::Type* type);
  FunctionSymbol(const std::string& symbolName, node::Type* type);
  FunctionSymbol(const char* symbolName, node::Type* type);
  virtual ~FunctionSymbol() = default;

  virtual std::string name() override;

  node::FunctionPrototype* prototype();
  void setPrototype(node::FunctionPrototype* prototype);
};

} // namespace symbol
} // namespace ast

#endif
