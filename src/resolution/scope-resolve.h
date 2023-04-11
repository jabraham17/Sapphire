#ifndef SAPPHIRE_RESOLUTION_SCOPE_RESOLVE_H_
#define SAPPHIRE_RESOLUTION_SCOPE_RESOLVE_H_

#include "ast/ast.h"
#include "ast/visitors/ast-visitor.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace pass {
namespace resolution {

using SymbolMap = std::unordered_map<std::string, ast::Symbol*>;

class ScopeResolve {
private:
  ast::ASTNode* root;
  std::vector<std::string> errors_;

public:
  ScopeResolve(ast::ASTNode* root) : root(root), errors_() {}
  virtual ~ScopeResolve() = default;
  bool hasErrors() { return !errors_.empty(); }
  std::vector<std::string> errors() { return errors_; }
  void resolve();
};

} // namespace resolution
} // namespace pass

#endif
