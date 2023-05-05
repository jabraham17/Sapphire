#ifndef SAPPHIRE_PASSES_RESOLUTION_SCOPE_RESOLVE_H_
#define SAPPHIRE_PASSES_RESOLUTION_SCOPE_RESOLVE_H_

#include "ast/ast.h"
#include "ast/node/type/nodes.h"
#include "passes/ast-pass.h"

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace pass {
namespace resolution {

using SymbolMap = std::unordered_map<std::string, ast::symbol::Symbol*>;
// using FunctionSymbolMap =
// std::unordered_map<ast::Ty, ast::symbol::FunctionSymbol*>;
using FunctionSymbolList = std::vector<ast::symbol::FunctionSymbol*>;

class ScopeResolve : public ASTPass {
public:
  ScopeResolve(ast::node::ASTNode* ast) : ASTPass(ast) {}
  virtual ~ScopeResolve() = default;
  virtual bool run() override;
};

} // namespace resolution
} // namespace pass

#endif
