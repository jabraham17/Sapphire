#ifndef HHH
#define HHH

#include "ast/ast.h"
#include "ast/node/nodes.h"
#include "ast/visitor/ast-visitor.h"

#include <iostream>
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
  bool hasReturn(ast::node::IfStatement* ifStmt) {
    bool hasReturns = false;
    hasReturns |= hasReturn(ifStmt->ifBody());
    if(ifStmt->hasPlainElse()) hasReturns |= hasReturn(ifStmt->plainElseBody());
    else if(ifStmt->hasElseIf()) hasReturns |= hasReturn(ifStmt->elseIfBody());
    return hasReturns;
  }
  // todo: this will be greatly improved by having a children iterator
  bool hasReturn(ast::node::Scope* scope) {
    bool hasReturns = false;
    for(auto stmt : *scope->statements()) {
      if(ast::isReturnStatementNode(stmt)) hasReturns = true;
      else if(auto f = ast::toForStatementNode(stmt); f != nullptr)
        hasReturns |= hasReturn(f->body());
      else if(auto i = ast::toIfStatementNode(stmt); i != nullptr)
        hasReturns |= hasReturn(i);
      else if(auto i = ast::toWhileStatementNode(stmt); i != nullptr)
        hasReturns |= hasReturn(i->body());
      else if(auto i = ast::toScopeNode(stmt); i != nullptr)
        hasReturns |= hasReturn(i);
    }
    return hasReturns;
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
