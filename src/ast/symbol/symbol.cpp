#include "symbol.h"

#include "ast/node/type/PrimitiveType.h"
#include "ast/node/type/Type.h"

#include <cassert>
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

std::string Symbol::basename() const { return name(); }
std::string Symbol::name() const { return symbolName; }
node::Type* Symbol::type() const { return type_; }

void Symbol::setName(const std::string& name) { this->symbolName = name; }
void Symbol::setType(node::Type* t) { this->type_ = t; }

std::string Symbol::toString(bool typed, bool showFlags) {
  std::string s = this->basename();
  if(showFlags) s += "{" + getFlagString(this->flags()) + "}";
  if(typed) s += ": " + this->type()->toString();
  return s;
}

void Symbol::setFlag(Flag flag, bool value) {
  auto idx = (std::size_t)flag;
  assert(idx >= 1 && idx <= this->flags().size());
  this->flags_.set(idx, value);
}
bool Symbol::getFlag(Flag flag) {
  auto idx = (std::size_t)flag;
  assert(idx >= 1 && idx <= this->flags().size());
  return this->flags().test(idx);
}
FlagSet Symbol::flags() { return this->flags_; }

bool Symbol::operator==(const Symbol& other) {
  return this->basename() == other.basename() &&
         node::Type::isSameType(this->type(), other.type());
}

} // namespace symbol
} // namespace ast
