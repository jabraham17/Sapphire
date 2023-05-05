#include "Nil.h"

#include "ast/node/type/PrimitiveType.h"
#include "ast/node/type/Type.h"

#include <cassert>

namespace ast {
namespace node {

// ASTNode* Nil::clone() {
//   auto x = new Nil();
//   x->isUserSpecified_ = isUserSpecified_;
//   return x;
// }
Nil::Nil(long line) : Nil() { setLine(line); }
Nil::Nil() : Nil(false) {}
Nil::Nil(long line, bool isUserSpecified) : Nil(isUserSpecified) {
  setLine(line);
}
Nil::Nil(bool isUserSpecified) : isUserSpecified_(isUserSpecified) {}

Type* Nil::type() { return Type::getNilType(); }
void Nil::setType([[maybe_unused]] Type* type) {
  assert(false && "cannot set the type of a literal nil");
}

bool Nil::isUserSpecified() { return this->isUserSpecified_; }
} // namespace node
} // namespace ast
