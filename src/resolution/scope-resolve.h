#ifndef SAPPHIRE_RESOLUTION_SCOPE_RESOLVE_H_
#define SAPPHIRE_RESOLUTION_SCOPE_RESOLVE_H_

#include "ast/ast.h"
#include "ast/node/type/nodes.h"

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "resolve-base.h"

namespace pass {
namespace resolution {

using SymbolMap = std::unordered_map<std::string, ast::symbol::Symbol*>;
// using FunctionSymbolMap =
// std::unordered_map<ast::Ty, ast::symbol::FunctionSymbol*>;
using FunctionSymbolList = std::vector<ast::symbol::FunctionSymbol*>;

class ScopeResolve : public ResolveBase {
public:
  ScopeResolve(ast::node::ASTNode* ast) : ResolveBase(ast) {}
  virtual ~ScopeResolve() = default;
  virtual bool resolve() override;
};

} // namespace resolution
} // namespace pass

#endif
