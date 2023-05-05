#ifndef SAPPHIRE_PASSES_RESOLUTION_TYPE_RESOLVE_H_
#define SAPPHIRE_PASSES_RESOLUTION_TYPE_RESOLVE_H_

#include "passes/ast-pass.h"

namespace pass {
namespace resolution {

class TypeResolve : public ASTPass {
public:
  TypeResolve(ast::node::ASTNode* ast) : ASTPass(ast) {}
  virtual ~TypeResolve() = default;
  virtual bool run() override;
};

} // namespace resolution
} // namespace pass
#endif
