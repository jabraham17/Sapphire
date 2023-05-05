#ifndef SAPPHIRE_PASSES_AST_PASS_H_
#define SAPPHIRE_PASSES_AST_PASS_H_

#include "ast/ast.h"
#include "ast/visitor/ast-visitor.h"

#include <string>
#include <vector>

namespace pass {
// base class for passes that modify an ast

class ASTPass {
protected:
  ast::node::ASTNode* ast;
  std::vector<std::string> errors_;

public:
  ASTPass(ast::node::ASTNode* ast) : ast(ast), errors_() {}
  virtual ~ASTPass() = default;

public:
  const std::vector<std::string>& errors() { return this->errors_; }
  void addError(const std::string& s) { this->errors_.push_back(s); }
  template <class InputIT> void addErrors(InputIT begin, InputIT end) {
    this->errors_.insert(this->errors_.end(), begin, end);
  }
  bool hasErrors() { return !this->errors_.empty(); }

  virtual bool run() = 0;
};

} // namespace pass
#endif
