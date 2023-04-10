
#include "codegen.h"

#include "codegen-visitor.h"

namespace codegen {
std::string doCodegen(ast::ASTNode* ast) {
  auto v = new CodegenVisitor();
  ast->accept(v);
  return v->getIR();
}
} // namespace codegen
