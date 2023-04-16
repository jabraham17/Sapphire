#ifndef SAPPHIRE_RESOLUTION_SCOPE_RESOLVE_H_
#define SAPPHIRE_RESOLUTION_SCOPE_RESOLVE_H_

#include "ast/ast.h"
#include "ast/visitor/ast-visitor.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace pass {
namespace resolution {

using SymbolMap = std::unordered_map<std::string, ast::symbol::Symbol*>;
using FunctionSymbolMap =
    std::unordered_map<std::string, ast::symbol::FunctionSymbol*>;

class ScopeResolve {
private:
  ast::node::ASTNode* root;
  std::vector<std::string> errors_;

public:
  ScopeResolve(ast::node::ASTNode* root) : root(root), errors_() {}
  virtual ~ScopeResolve() = default;
  bool hasErrors() { return !errors_.empty(); }
  std::vector<std::string> errors() { return errors_; }
  void resolve();
};

} // namespace resolution
} // namespace pass

#endif
