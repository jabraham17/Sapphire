#include "resolve.h"

#if defined(DEBUG) && DEBUG == 1
  #include "ast/visitor/ast-dump-tree.h"
  #include "ast/visitor/ast-dump.h"

  #include <iostream>
#endif

#include "scope-resolve.h"
#include "type-resolve.h"

namespace pass {
namespace resolution {
bool Resolve::resolve() {

#if defined(DEBUG) && DEBUG == 1
  std::cerr << "Initial AST\n";
  ast->accept(new ast::visitor::ASTDump(std::cerr));
  std::cerr << std::endl;
  std::cerr << std::string(80, '=') << std::endl;
#endif

  for(std::size_t i = 0; i < recursiveDepth; i++) {

    // resolve scopes
    {
      pass::resolution::ScopeResolve sr(ast);

      if(!sr.resolve()) {
        addErrors(sr.errors().begin(), sr.errors().end());

#if defined(DEBUG) && DEBUG == 1
        std::cerr << "Failed to scope resolve at this point...\n";
        ast->accept(new ast::visitor::ASTDump(std::cerr));
        std::cerr << std::endl;
        std::cerr << std::string(80, '=') << std::endl;
#endif

        return 1;
      }
    }

    // resolve types
    {
      pass::resolution::TypeResolve tr(ast);

      if(!tr.resolve()) {
        addErrors(tr.errors().begin(), tr.errors().end());

#if defined(DEBUG) && DEBUG == 1
        std::cerr << "Failed to type resolve at this point...\n";
        ast->accept(new ast::visitor::ASTDump(std::cerr));
        std::cerr << std::endl;
        std::cerr << std::string(80, '=') << std::endl;
#endif

        return 1;
      }
    }
  }
// resolved code
#if defined(DEBUG) && DEBUG == 1
  std::cerr << "Resolved ast\n";
  ast->accept(new ast::visitor::ASTDump(std::cerr));
  std::cerr << std::endl;
  std::cerr << std::string(80, '=') << std::endl;
#endif

  return hasErrors();
}
} // namespace resolution
} // namespace pass
