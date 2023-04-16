#ifndef SAPPHIRE_PARSER_PARSER_H_
#define SAPPHIRE_PARSER_PARSER_H_

#include "ast/ast.h"

#include <string>
#include <vector>

namespace parser {

struct Context {
private:
  std::vector<std::string> errorMessage_;

public:
  ast::node::ASTNode* ast;

public:
  Context();
  void addError(const char* s);
  void addError(const std::string& s);
  bool hasErrors();
  std::vector<std::string> errors();
};

ast::node::ASTNode* parse(FILE* fp);

} // namespace parser

#endif
