#ifndef SAPPHIRE_RESOLUTION_TYPE_RESOLVE_H_
#define SAPPHIRE_RESOLUTION_TYPE_RESOLVE_H_

#include "resolve-base.h"

namespace pass {
namespace resolution {

class TypeResolve : public ResolveBase {
public:
  TypeResolve(ast::node::ASTNode* ast) : ResolveBase(ast) {}
  virtual ~TypeResolve() = default;
  virtual bool resolve() override;
};

} // namespace resolution
} // namespace pass
#endif
