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
      // TODO: handle fields of the type, not yet handled
      // a function gets resolved from (exp.sym) to (call sym, exp, ...)
      // TODO: handle what if the next operands is not a call?

      // case ast::node::OperatorType::FIELD_ACCESS: {

      //   break;
      // }
      case ast::node::OperatorType::FUNCTION: {
        ast::node::UseExpression* callFuncExpr = nullptr;
        // if its a use, directly get it
        if(call->operands().get(0)->isUseExpression()) {
          callFuncExpr = call->operands().get(0)->toUseExpression();
        } else if(auto fieldAccess =
                      call->operands().get(0)->toCallExpression();
                  fieldAccess != nullptr &&
                  fieldAccess->opType() ==
                      ast::node::OperatorType::FIELD_ACCESS) {
          // if the "symbol" is actually a field access, we replace it with its
          // components resolve op0
          auto op0 = fieldAccess->operands().get(0)->toExpression();
          assert(op0 != nullptr);
          op0->accept(this);
          // resolve op1, it MUST be a useExpression
          auto op1 = fieldAccess->operands().get(1)->toUseExpression();
          assert(op1 != nullptr);

          // add 'op1, op0' to operands, do this by adding to the front and
          // replacing
          auto newCall =
              new ast::node::CallExpression(call->line(), call->opType());
          newCall->addOperand(op1);
          newCall->addOperand(op0);
          // skip the old first op
          auto ops = call->operands();
          for(auto it = ops.begin() + 1; it != ops.end(); it++) {
            newCall->addOperand(*it);
          }
          call->replaceWith(newCall);
          call = newCall;

          // now we have a symbol that can be resolved
          callFuncExpr = call->operands().get(0)->toUseExpression();

        } else {
          assert(false);
        }

        // we need to resolve the other operands to make this work nicely
        // resolve remaining operands
        for(auto it = call->operands().begin() + 1;
            it != call->operands().end();
            it++) {
          (*it)->accept(this);
        }

        // search through all functions
        auto callFuncSym = callFuncExpr->symbol();
        ast::symbol::FunctionSymbol* found = nullptr;
        for(auto funcSym : function_symbols) {
          // std::cerr << "cmp " << funcSym->basename() << " "
          //           << callFuncSym->basename() << "\n";
          // they are the same if
          // 1. they have the same name
          // 2. the number and types of the call expression match the prototype

          if(funcSym->basename() == callFuncSym->basename()) {
            // std::cerr << funcSym->basename() << " basename\n";
            auto funcSig = funcSym->type()->toCallableType();
            auto funcParamTypes = funcSig->parameterTypes();
            // std::cerr << "nops" << call->operands().size() << std::endl;
            if(funcParamTypes.size() != (call->operands().size() - 1)) continue;
            bool matched = true;

            for(auto it = call->operands().begin() + 1;
                it != call->operands().end();
                it++) {
              auto idx = (it - call->operands().begin()) - 1;
              auto operand = (*it)->toExpression();
              assert(operand != nullptr);
              // std::cerr << "    cmp " <<
              // funcParamTypes.get(idx)->toString() << " "
              // << operand->type()->toString() << "\n";
              if(!ast::node::Type::isSameType(
                     operand->type(),
                     *funcParamTypes.get(idx))) {
                matched = false;
              }
            }
            // std::cerr << "did we match" << matched << "\n";
            if(!matched) continue;
            // std::cerr << "found it \n";
            // we found a match!
            found = funcSym;
            break;
          }
        }
        if(found) {
          assert(callFuncExpr->isUseExpression());
          callFuncExpr->toUseExpression()->setSymbol(found);
        } else {
          this->returnValue_.push_back(
              "unable to resolve function call to '" + callFuncSym->toString() +
              "' on line " + std::to_string(call->line()));
        }

        break;
      }
      default:
        for(auto c : call->operands())
          c->accept(this);
        break;
    }
  }

  // on another scope, call a new resolve one scope
  virtual void visitImpl(ast::node::Scope* subScope) override {
    auto function_symbols = get<0>();
    auto symbols = get<1>();
    //  no changes to our symbols should occur, this subSAcope gets a copy
    ResolveOneScope ros(function_symbols, symbols);
    for(auto stmt : subScope->statements())
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
    auto funcName = func->functionPrototype()->symbol()->basename();

    for(auto param : func->functionPrototype()->parameters()) {
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

bool ScopeResolve::run() {
  FunctionSymbolList function_symbols;
  {
    BuildFunctionSymbols bfs(&function_symbols);
    ast->accept(&bfs);
    auto functionErrors = bfs.returnValueAndClear();
    addErrors(functionErrors.begin(), functionErrors.end());
  }
#if defined(DEBUG) && DEBUG == 1
  std::cerr << std::string(80, '=') << "\n";
  std::cerr << "Function symbol table\n";
  for(auto func : function_symbols) {
    std::cerr << "  " << func->toString(true) << "\n";
  }
  std::cerr << std::string(80, '=') << "\n";
#endif
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
