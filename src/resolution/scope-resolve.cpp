#include "scope-resolve.h"

#include "ast/node/nodes.h"
#include "ast/symbol/function-symbol.h"
#include "ast/symbol/symbol.h"

#include <cassert>
#include <iostream>
#include <unordered_map>

namespace pass {
namespace resolution {

class BuildFunctionSymbols final
    : public ast::visitor::VisitorWithArgsAndReturn<
          BuildFunctionSymbols,
          ast::visitor::VisitAll,
          std::vector<std::string>,
          FunctionSymbolList*> {
public:
  using VisitorWithArgsAndReturn::VisitorWithArgsAndReturn;

protected:
  virtual void visitImpl(ast::node::FunctionPrototype* proto) override {

    auto function_symbols = get<0>();

    auto sym = proto->symbol();
    auto name = sym->basename();

    for(auto func : *function_symbols) {
      if(*sym == *func) {
        this->returnValue_.push_back(
            "cannot redefine function '" + name + "' on line " +
            std::to_string(proto->line()));
        return;
      }
    }

    // no error, insert to table
    function_symbols->push_back(sym);
  }
};

// traverse the node and find a symbol
// static Symbol* getSymbol(ast::node::ASTNode* node) {

// }

// returns any errors
// takes a map of function_symbols and symbols
// this is modified as we execute, but it does not affect the outside scope
class ResolveOneScope final : public ast::visitor::VisitorWithArgsAndReturn<
                                  ResolveOneScope,
                                  ast::visitor::VisitAll,
                                  std::vector<std::string>,
                                  FunctionSymbolList,
                                  SymbolMap> {
public:
  using VisitorWithArgsAndReturn::VisitorWithArgsAndReturn;

protected:
  // on a def: add to table of vars
  virtual void visitImpl(ast::node::DefExpression* def) override {
    // need a pointer so we update variables
    auto& symbols = get<1>();

    auto sym = def->symbol();
    auto name = sym->name();

    // TODO: we can probably do insert_or_assign
    if(auto existing = symbols.find(name); existing == symbols.end()) {
      // does not exist already, no error add it
      symbols.insert({name, sym});
    } else {
      // already exists: we shadow the previous def
      symbols.at(name) = sym;
    }

    // also resolve def body
    if(def->hasInitialValue()) {
      def->initialValue()->accept(this);
    }
  }
  // on a use: resolve to the table of values if possible, otherwise error
  virtual void visitImpl(ast::node::UseExpression* use) override {
    // need a pointer so we update symbols
    auto symbols = get<1>();

    auto sym = use->symbol();
    auto name = sym->name();

    if(auto existing = symbols.find(name); existing != symbols.end()) {
      // already exists, resolve
      sym = existing->second;
      use->setSymbol(sym);
    } else {
      // did not exists, error
      this->returnValue_.push_back(
          "unable to resolve symbol '" + name + "' on line " +
          std::to_string(use->line()));
    }
  }

  // on call expression, resolve the function call
  virtual void visitImpl(ast::node::CallExpression* call) override {
    auto function_symbols = get<0>();

    switch(call->opType()) {
      // field access, either its a function of a type or a field of a type.
      // TODO: handle fields of the type
      // a function gets resolved from (exp.sym) to (call sym, exp, ...)
      // we can handle the call expression as a curry, then whoever uses this
      // has to handle the currying

      // case ast::node::OperatorType::FIELD_ACCESS: {
      //   // resolve op0
      //   auto op0 = ast::toExpressionNode(call->operands()->get(0));
      //   assert(op0 != nullptr);
      //   op0->accept(this);

      //   // get op1, it MUST be a useExpression
      //   auto op1 = ast::toUseExpressionNode(call->operands()->get(1));
      //   assert(op1 != nullptr);

      //   break;
      // }
      case ast::node::OperatorType::FUNCTION: {
        auto funcNode = call->operands()->get(0);
        auto callFuncExpr = ast::toExpressionNode(call->operands()->get(0));
        assert(callFuncExpr != nullptr);
        ast::symbol::Symbol* callFuncSym = nullptr;
        std::string callFuncName;
        // if its a use, directly get it
        if(ast::isUseExpressionNode(callFuncExpr)) {
          callFuncSym = ast::toUseExpressionNode(callFuncExpr)->symbol();
          callFuncName = callFuncSym->name();
        }

        // if(auto fieldAccess = ast::toCallExpressionNode(callFuncExpr);
        //           fieldAccess != nullptr &&
        //           fieldAccess->opType() ==
        //               ast::node::OperatorType::FIELD_ACCESS) {
        //   // if its a field access
        //   // TODO: to resolve this, I HAVE to know its type
        //   // that way i know which function to insert here
        //   // but type resolution happes after
        //   // maybe after i scope resolve each statment, i immediatly type
        //   // resolve it? leapfrogging.

        //   // field access is a expr DOT useexpr
        //   // auto op0 = ast::toExpressionNode(call->operands()->get(0));
        //   // auto op1 = ast::toUseExpressionNode(call->operands()->get(1));
        //   // assert(op0 != nullptr && op1 != nullptr);

        //   // // construct a possible callFunc
        //   // callFuncSym = new ast::symbol::FunctionSymbol(
        //   //     op1->symbol()->name(),
        //   //     op0->type());
        //   // if this matches, we will need to modify
        //   // callFuncName
        // } else {
        //   assert(false);
        // }
        // auto callFuncExpr =
        // ast::toUseExpressionNode(call->operands()->get(0)); if(callFuncExpr
        // == nullptr) {
        //   // not a use expr, try and resolve and then try again
        //   call->operands()->get(0)->accept(this);

        // }
        // assert(callFuncExpr != nullptr);

        // search through all functions
        ast::symbol::FunctionSymbol* found = nullptr;
        for(auto funcSym : function_symbols) {
          // they are the same if
          // 1. they have the same name
          // 2. the number and types of the call expression match the prototype

          // if(callFuncName == funcSym) {
          //   found = funcSym.second;
          //   break;
          // }
        }
        if(found) {
          assert(ast::isUseExpressionNode(callFuncExpr));
          ast::toUseExpressionNode(callFuncExpr)->setSymbol(found);
        } else {
          this->returnValue_.push_back(
              "unable to resolve function '" + callFuncName + "' on line " +
              std::to_string(call->line()));
        }

        // resolve remaining operands
        for(auto it = call->operands()->begin() + 1;
            it != call->operands()->end();
            it++) {
          (*it)->accept(this);
        }
        break;
      }
      default: call->operands()->accept(this); break;
    }
  }

  // on another scope, call a new resolve one scope
  virtual void visitImpl(ast::node::Scope* subScope) override {
    auto function_symbols = get<0>();
    auto symbols = get<1>();
    //  no changes to our symbols should occur, this subSAcope gets a copy
    ResolveOneScope ros(function_symbols, symbols);
    for(auto stmt : *subScope->statements())
      stmt->accept(&ros);
    auto subScopeErrors = ros.returnValueAndClear();

    this->returnValue_.insert(
        this->returnValue_.end(),
        subScopeErrors.begin(),
        subScopeErrors.end());
  }
};

// returns any errors
// takes a map of function_symbols
// this is modified as we execute, but it does not affect the outside scope
class ResolveFunctions final : public ast::visitor::VisitorWithArgsAndReturn<
                                   ResolveFunctions,
                                   ast::visitor::VisitAll,
                                   std::vector<std::string>,
                                   FunctionSymbolList> {
public:
  using VisitorWithArgsAndReturn::VisitorWithArgsAndReturn;

protected:
  void visitImpl(ast::node::FunctionDefinition* func) {
    auto function_symbols = get<0>();
    SymbolMap symbols;
    auto funcName = func->functionPrototype()->name();

    for(auto elm : *func->functionPrototype()->parameters()) {
      if(auto param = ast::toParameterNode(elm); param != nullptr) {
        auto name = param->symbol()->name();
        if(auto existing = symbols.find(name); existing == symbols.end()) {
          // does not exist already, no error add it
          symbols.insert({name, param->symbol()});
        } else {
          this->returnValue_.push_back(
              "duplicate parameter '" + name + "' in function '" + funcName +
              "' on line " + std::to_string(param->line()));
          return;
        }
      } else {
        this->returnValue_.push_back(
            "invalid parameter in function '" + funcName + "' on line " +
            std::to_string(func->line()));
        return;
      }
    }

    // visit each statement and apply the scope resolution
    ResolveOneScope ros(function_symbols, symbols);
    func->accept(&ros);
    auto scopeErrors = ros.returnValueAndClear();

    for(auto e : scopeErrors) {
      this->returnValue_.push_back("  in '" + funcName + "': " + e);
    }
  }
};

bool ScopeResolve::resolve() {
  FunctionSymbolList function_symbols;
  {
    BuildFunctionSymbols bfs(&function_symbols);
    ast->accept(&bfs);
    auto functionErrors = bfs.returnValueAndClear();
    addErrors(functionErrors.begin(), functionErrors.end());
  }
  {
    ResolveFunctions rf(function_symbols);
    ast->accept(&rf);
    auto functionErrors = rf.returnValueAndClear();
    addErrors(functionErrors.begin(), functionErrors.end());
  }
  return this->hasErrors();
}
} // namespace resolution
} // namespace pass
