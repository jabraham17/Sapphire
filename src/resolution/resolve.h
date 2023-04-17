#ifndef SAPPHIRE_RESOLUTION_RESOLVE_H_
#define SAPPHIRE_RESOLUTION_RESOLVE_H_

#include "resolve-base.h"

namespace pass {
namespace resolution {

class Resolve : public ResolveBase {
private:
  std::size_t recursiveDepth = 2;

public:
  Resolve(ast::node::ASTNode* ast, std::size_t recursiveDepth = 2)
      : ResolveBase(ast), recursiveDepth(recursiveDepth) {}
  virtual ~Resolve() = default;
  virtual bool resolve() override;
};

} // namespace resolution
} // namespace pass

#endif
