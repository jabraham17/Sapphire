
#include "passes/simple-error-checking.h"

#include "check-return.hpp"

namespace pass {
namespace error_checking {
bool SimpleErrorChecking::run() {
  {
    CheckReturnStatements vis;
    ast->accept(&vis);
    auto errors = vis.returnValueAndClear();
    addErrors(errors.begin(), errors.end());
  }
  return this->hasErrors();
}
} // namespace error_checking
} // namespace pass
