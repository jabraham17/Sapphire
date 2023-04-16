#ifndef SAPPHIRE_RESOLUTION_TYPE_RESOLVE_H_
#define SAPPHIRE_RESOLUTION_TYPE_RESOLVE_H_

#include "ast/ast.h"
#include "ast/visitor/ast-visitor.h"

#include <string>
#include <vector>

namespace pass {
namespace resolution {

class TypeResolve {
private:
  ast::node::ASTNode* root;
  std::vector<std::string> errors_;

public:
  TypeResolve(ast::node::ASTNode* root) : root(root), errors_() {}
  virtual ~TypeResolve() = default;
  bool hasErrors() { return !errors_.empty(); }
  std::vector<std::string> errors() { return errors_; }
  void resolve();
};

} // namespace resolution
} // namespace pass
#endif
