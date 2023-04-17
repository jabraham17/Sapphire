#ifndef SAPPHIRE_AST_SYMBOL_FUNCTION_SYMBOL_H_
#define SAPPHIRE_AST_SYMBOL_FUNCTION_SYMBOL_H_

#include "symbol.h"

#include "ast/ast.h"

#include <string>

namespace ast {

namespace symbol {

class FunctionSymbol : public Symbol {
  node::Type* belongsTo_;
  node::FunctionPrototype* prototype_;

public:
  FunctionSymbol(
      node::FunctionPrototype* prototype,
      const std::string& symbolName,
      node::Type* belongsTo,
      node::Type* type);
  FunctionSymbol(
      node::FunctionPrototype* prototype,
      const char* symbolName,
      node::Type* belongsTo,
      node::Type* type);
  FunctionSymbol(const std::string& symbolName, node::Type* type);
  FunctionSymbol(const char* symbolName, node::Type* type);
  virtual ~FunctionSymbol() = default;

  virtual std::string name() const override;

  virtual std::string
  toString(bool typed = false, bool showFlags = false) override;

  node::FunctionPrototype* prototype() const;
  node::FunctionPrototype* prototype() {
    return const_cast<const FunctionSymbol*>(this)->prototype();
  }
  void setPrototype(node::FunctionPrototype* prototype);

  node::Type* belongsTo() const;
  node::Type* belongsTo() {
    return const_cast<const FunctionSymbol*>(this)->belongsTo();
  }
  void setBelongsTo(node::Type* belongsTo);

  bool operator==(const FunctionSymbol& other);
};

} // namespace symbol
} // namespace ast

#endif
