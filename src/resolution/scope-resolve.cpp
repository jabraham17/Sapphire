#include "scope-resolve.h"

#include <iostream>
#include <unordered_map>

namespace pass {
namespace resolution {

class BuildFunctionSymbols final
    : public ast::visitor::VisitorWithArgsAndReturn<
          BuildFunctionSymbols,
          ast::visitor::VisitAll,
          std::vector<std::string>,
          SymbolMap*> {
public:
  using VisitorWithArgsAndReturn::VisitorWithArgsAndReturn;

protected:
  virtual void visitImpl(ast::FunctionPrototype* proto) override {

    auto symbols = get<0>();

    auto sym = proto->functionSymbol();
    auto name = sym->name();

    // TODO: we can probably do insert_or_assign
    if(auto existing = symbols->find(name); existing == symbols->end()) {
      // does not exist already, no error add it
      symbols->insert({name, sym});
    } else {
      this->returnValue_.push_back(
          "cannot redefine function '" + name + "' on line " +
          std::to_string(proto->line()));
    }
  }
};

// returns any errors
// takes a map of variables
// this is modified as we execute, but it does not affect the outside scope
class ResolveOneScope final : public ast::visitor::VisitorWithArgsAndReturn<
                                  ResolveOneScope,
                                  ast::visitor::VisitAll,
                                  std::vector<std::string>,
                                  SymbolMap> {
public:
  using VisitorWithArgsAndReturn::VisitorWithArgsAndReturn;

protected:
  // on a def: add to table of vars
  virtual void visitImpl(ast::DefExpression* def) override {
    // need a pointer so we update variables
    auto& symbols = get<0>();

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
      def->assignValue()->accept(this);
    }
  }
  // on a use: resolve to the table of values if possible, otherwise error
  virtual void visitImpl(ast::UseExpression* use) override {
    // need a pointer so we update symbols
    auto symbols = get<0>();

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

  // on another scope, call a new resolve one scope
  virtual void visitImpl(ast::Scope* subScope) override {
    auto symbols = get<0>();
    //  no changes to our symbols should occur, this subSAcope gets a copy
    ResolveOneScope ros(symbols);
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
// takes a map of variables
// this is modified as we execute, but it does not affect the outside scope
class ResolveFunctions final : public ast::visitor::VisitorWithArgsAndReturn<
                                   ResolveFunctions,
                                   ast::visitor::VisitAll,
                                   std::vector<std::string>,
                                   SymbolMap> {
public:
  using VisitorWithArgsAndReturn::VisitorWithArgsAndReturn;

protected:
  void visitImpl(ast::FunctionDefinition* func) {
    auto symbols = get<0>();
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
    ResolveOneScope ros(symbols);
    func->accept(&ros);
    auto scopeErrors = ros.returnValueAndClear();

    if(!scopeErrors.empty()) {
      for(auto e : scopeErrors) {
        this->returnValue_.push_back("  in '" + funcName + "': " + e);
      }
    }
  }
};

void ScopeResolve::resolve() {
  SymbolMap symbols;

  BuildFunctionSymbols bfs(&symbols);
  root->accept(&bfs);
  auto functionErrors = bfs.returnValueAndClear();
  this->errors_.insert(
      this->errors_.end(),
      functionErrors.begin(),
      functionErrors.end());

  ResolveFunctions rf(symbols);
  root->accept(&rf);
  functionErrors = bfs.returnValueAndClear();
  this->errors_.insert(
      this->errors_.end(),
      functionErrors.begin(),
      functionErrors.end());
}
} // namespace resolution
} // namespace pass
