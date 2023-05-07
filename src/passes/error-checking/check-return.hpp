#ifndef SAPPHIRE_PASSES_ERROR_CHECKING_CHECK_RETURN_H_
#define SAPPHIRE_PASSES_ERROR_CHECKING_CHECK_RETURN_H_

#include "ast/ast.h"
#include "ast/node/nodes.h"
#include "ast/visitor/ast-visitor.h"

#include <iostream>
#include <queue>
#include <unordered_map>

namespace pass {
namespace error_checking {
class CheckReturnStatements final
    : public ast::visitor::VisitorWithArgsAndReturn<
          CheckReturnStatements,
          ast::visitor::VisitAll,
          std::vector<std::string>> {
public:
  using VisitorWithArgsAndReturn::VisitorWithArgsAndReturn;

protected:
  bool hasReturn(ast::node::ASTNode* node) {
    std::queue<ast::node::ASTNode*> q;
    q.push(node);
    while(!q.empty()) {
      auto n = q.front();
      q.pop();
      for(auto stmt : n->children()) {
        if(stmt->isReturnStatement()) return true;
        q.push(stmt);
      }
    }
    return false;
  }

  virtual void visitImpl(ast::node::FunctionDefinition* func) override {

    auto retType = func->functionPrototype()->type()->returnType();
    bool shouldHaveReturn = !(retType->isNilType());

    bool hasReturns = hasReturn(func->body());

    if(hasReturns && !shouldHaveReturn) {
      this->returnValue_.push_back(
          "function '" + func->functionPrototype()->name() + "' on line " +
          std::to_string(func->functionPrototype()->line()) +
          " should not have a return statement");
      return;
    }
    if(!hasReturns && shouldHaveReturn) {
      this->returnValue_.push_back(
          "function '" + func->functionPrototype()->name() + "' on line " +
          std::to_string(func->functionPrototype()->line()) +
          " should have a return statement");
      return;
    }
  }
};
} // namespace error_checking
} // namespace pass

#endif
