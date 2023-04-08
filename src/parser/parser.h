#ifndef SAPPHIRE_PARSER_PARSER_H_
#define SAPPHIRE_PARSER_PARSER_H_

#include "ast/ast.h"

#include <memory>

namespace parser {

ast::ASTNode* parse(FILE* fp);

}

#endif
