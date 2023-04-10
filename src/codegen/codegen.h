#ifndef SAPPHIRE_CODEGEN_CODEGEN_H_
#define SAPPHIRE_CODEGEN_CODEGEN_H_

#include "ast/ast.h"

namespace codegen {

std::string doCodegen(ast::ASTNode* ast);

}

#endif
