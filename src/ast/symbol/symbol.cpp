#include "symbol.h"

#include "ast/node/type/PrimitiveType.h"
#include "ast/node/type/Type.h"
namespace ast {

namespace symbol {

Symbol::Symbol(const std::string& symbolName)
    : Symbol(symbolName, node::Type::getUnknownType()) {}

Symbol::Symbol(const char* symbolName)
    : Symbol(symbolName, node::Type::getUnknownType()) {}

Symbol::Symbol(const std::string& symbolName, node::Type* type)
    : symbolName(symbolName), type_(type) {}

Symbol::Symbol(const char* symbolName, node::Type* type)
    : symbolName(symbolName), type_(type) {}

std::string Symbol::basename() { return name(); }
std::string Symbol::name() { return symbolName; }
node::Type* Symbol::type() { return type_; }

void Symbol::setName(const std::string& name) { this->symbolName = name; }
void Symbol::setType(node::Type* t) { this->type_ = t; }

std::string Symbol::toString() {
  return this->basename() + ": " + this->type_->toString();
}

} // namespace symbol
} // namespace ast
