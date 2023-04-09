#ifndef SAPPHIRE_PARSER_PARSER_H_
#define SAPPHIRE_PARSER_PARSER_H_

#include "ast/ast.h"

#include <vector>

namespace parser {

struct Context {
private:
  std::vector<std::string> errorMessage_;

public:
  ast::ASTNode* ast;

public:
  Context() : errorMessage_() {}
  void addError(const char* s) { errorMessage_.push_back(s); }
  void addError(const std::string& s) { errorMessage_.push_back(s); }
  bool hasErrors() { return errorMessage_.size() > 0; }
  auto errors() { return errorMessage_; }
};

ast::ASTNode* parse(FILE* fp);

} // namespace parser

#endif
