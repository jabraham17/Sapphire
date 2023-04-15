#ifndef SAPPHIRE_AST_SYMBOL_SYMBOL_H_
#define SAPPHIRE_AST_SYMBOL_SYMBOL_H_

#include "ast/ast.h"

namespace ast {

namespace symbol {

class Symbol {
  std::string symbolName;
  node::Type* type_;

public:
  Symbol(const std::string& symbolName);
  Symbol(const char* symbolName);
  Symbol(const std::string& symbolName, node::Type* type);
  Symbol(const char* symbolName, node::Type* type);
  virtual ~Symbol() = default;

  virtual std::string basename();
  virtual std::string name();
  virtual node::Type* type();

  virtual void setName(const std::string& name);
  virtual void setType(node::Type* t);

  virtual std::string toString();
};

} // namespace symbol
} // namespace ast

#endif
