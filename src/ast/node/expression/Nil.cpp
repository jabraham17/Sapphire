#include "Nil.h"

#include "ast/node/type/PrimitiveType.h"
#include "ast/node/type/Type.h"

#include <cassert>

namespace ast {
namespace node {
Nil::Nil(long lineNumber) : Nil() { setLine(lineNumber); }
Nil::Nil() : Nil(false) {}
Nil::Nil(long lineNumber, bool isUserSpecified) : Nil(isUserSpecified) {
  setLine(lineNumber);
}
Nil::Nil(bool isUserSpecified) : isUserSpecified_(isUserSpecified) {}

Type* Nil::type() { return Type::getNilType(); }
void Nil::setType([[maybe_unused]] Type* type) {
  assert(false && "cannot set the type of a literal nil");
}

bool Nil::isUserSpecified() { return this->isUserSpecified_; }
} // namespace node
} // namespace ast
