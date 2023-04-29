#ifndef SAPPHIRE_RESOLUTION_RESOLVE_H_
#define SAPPHIRE_RESOLUTION_RESOLVE_H_

#include "ast-pass.h"

namespace pass {
namespace resolution {

class Resolve : public ASTPass {
private:
  std::size_t recursiveDepth = 2;

public:
  Resolve(ast::node::ASTNode* ast, std::size_t recursiveDepth = 2)
      : ASTPass(ast), recursiveDepth(recursiveDepth) {}
  virtual ~Resolve() = default;
  virtual bool run() override;
};

} // namespace resolution
} // namespace pass

#endif
