#ifndef SAPPHIRE_AST_SYMBOL_SYMBOL_H_
#define SAPPHIRE_AST_SYMBOL_SYMBOL_H_

#include "flags.h"

#include "ast/ast.h"

#include <string>

namespace ast {

namespace symbol {

class Symbol {
  std::string symbolName;
  node::Type* type_;

  FlagSet flags_;

public:
  Symbol(const std::string& symbolName);
  Symbol(const char* symbolName);
  Symbol(const std::string& symbolName, node::Type* type);
  Symbol(const char* symbolName, node::Type* type);
  virtual ~Symbol() = default;

  virtual std::string basename() const;
  virtual std::string name() const;
  virtual node::Type* type() const;

  std::string basename() { return const_cast<const Symbol*>(this)->basename(); }
  std::string name() { return const_cast<const Symbol*>(this)->name(); }
  node::Type* type() { return const_cast<const Symbol*>(this)->type(); }

  virtual void setName(const std::string& name);
  virtual void setType(node::Type* t);

  virtual std::string toString(bool typed = false, bool showFlags = false);

  void setFlag(Flag flag, bool value = true);
  bool getFlag(Flag flag);
  FlagSet flags();

  bool operator==(const Symbol& other);
};

} // namespace symbol
} // namespace ast

#endif
