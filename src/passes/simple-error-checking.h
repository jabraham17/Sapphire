#ifndef PP
#define PP

#include "ast-pass.h"

namespace pass {
namespace error_checking {
class SimpleErrorChecking : public ASTPass {

public:
  SimpleErrorChecking(ast::node::ASTNode* ast) : ASTPass(ast) {}
  virtual ~SimpleErrorChecking() = default;
  virtual bool run() override;
};
} // namespace error_checking
} // namespace pass

#endif
