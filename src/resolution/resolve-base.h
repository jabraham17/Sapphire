#ifndef SAPPHIRE_RESOLUTION_RESOLVE_BASE_H_
#define SAPPHIRE_RESOLUTION_RESOLVE_BASE_H_

#include "ast/ast.h"
#include "ast/visitor/ast-visitor.h"

#include <string>
#include <vector>

namespace pass {
namespace resolution {

class ResolveBase {
protected:
  ast::node::ASTNode* ast;
  std::vector<std::string> errors_;

public:
  ResolveBase(ast::node::ASTNode* ast) : ast(ast), errors_() {}
  virtual ~ResolveBase() = default;

public:
  std::vector<std::string> errors() { return this->errors_; }
  void addError(const std::string& s) { this->errors_.push_back(s); }
  template <class InputIT> void addErrors(InputIT begin, InputIT end) {
    this->errors_.insert(this->errors_.end(), begin, end);
  }
  bool hasErrors() { return !this->errors_.empty(); }

  virtual bool resolve() = 0;
};

} // namespace resolution
} // namespace pass

#endif
