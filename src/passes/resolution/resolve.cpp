#include "passes/resolve.h"

#if defined(DEBUG) && DEBUG == 1
  #include "ast/visitor/ast-dump-tree.h"
  #include "ast/visitor/ast-dump.h"

  #include <iostream>
#endif

#include "scope-resolve.h"
#include "type-resolve.h"

namespace pass {
namespace resolution {
bool Resolve::run() {

#if defined(DEBUG) && DEBUG == 1
  std::cerr << "DEBUG: Initial AST\n";
  {
    ast::visitor::ASTDump dump(std::cerr, 4);
    ast->accept(&dump);
  }
  std::cerr << std::endl;
  std::cerr << std::string(80, '=') << std::endl;
#endif

  for(std::size_t i = 0; i < recursiveDepth; i++) {

#if defined(DEBUG) && DEBUG == 1
    std::cerr << "Resolve Pass " << i << "\n";
#endif

    // resolve scopes
    {
      pass::resolution::ScopeResolve sr(ast);

      if(sr.run()) {
        addErrors(sr.errors().begin(), sr.errors().end());

#if defined(DEBUG) && DEBUG == 1
        std::cerr << "DEBUG: Failed to scope resolve at this point...\n";
        {
          ast::visitor::ASTDump dump(std::cerr, 4);
          ast->accept(&dump);
        }
        std::cerr << std::endl;
        std::cerr << std::string(80, '=') << std::endl;
#endif

        return hasErrors();
      }
    }

    // resolve types
    {
      pass::resolution::TypeResolve tr(ast);

      if(tr.run()) {
        addErrors(tr.errors().begin(), tr.errors().end());

#if defined(DEBUG) && DEBUG == 1
        std::cerr << "DEBUG: Failed to type resolve at this point...\n";
        {
          ast::visitor::ASTDump dump(std::cerr, 4);
          ast->accept(&dump);
        }
        std::cerr << std::endl;
        std::cerr << std::string(80, '=') << std::endl;
#endif

        return hasErrors();
      }
    }
  }
// resolved code
#if defined(DEBUG) && DEBUG == 1
  std::cerr << "DEBUG: Resolved ast\n";
  {
    ast::visitor::ASTDump dump(std::cerr, 4);
    ast->accept(&dump);
  }
  std::cerr << std::endl;
  std::cerr << std::string(80, '=') << std::endl;
#endif

  return hasErrors();
}
} // namespace resolution
} // namespace pass
