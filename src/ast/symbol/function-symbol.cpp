#include "symbol.h"

#include "ast/node/type/Type.h"

namespace ast {

namespace symbol {

FunctionSymbol::FunctionSymbol(
    node::FunctionPrototype* prototype,
    const std::string& symbolName,
    node::Type* type)
    : Symbol(symbolName, type), prototype_(prototype) {}

FunctionSymbol::FunctionSymbol(
    node::FunctionPrototype* prototype,
    const char* symbolName,
    node::Type* type)
    : Symbol(symbolName, type), prototype_(prototype) {}

FunctionSymbol::FunctionSymbol(const std::string& symbolName, node::Type* type)
    : FunctionSymbol(nullptr, symbolName, type) {}

FunctionSymbol::FunctionSymbol(const char* symbolName, node::Type* type)
    : FunctionSymbol(nullptr, symbolName, type) {}

std::string FunctionSymbol::name() {
  // TODO
}

node::FunctionPrototype* FunctionSymbol::prototype() {
  return this->prototype_;
};
void FunctionSymbol::setPrototype(node::FunctionPrototype* prototype) {
  this->prototype_ = prototype;
}

} // namespace symbol
} // namespace ast
