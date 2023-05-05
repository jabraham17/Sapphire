#include "RealExpression.h"

#include "ast/node/type/PrimitiveType.h"
#include "ast/node/type/Type.h"

#include <cassert>

namespace ast {
namespace node {

// ASTNode* RealExpression::clone() { return new RealExpression(value_); }
RealExpression::RealExpression(long line, value_type value)
    : RealExpression(value) {
  setLine(line);
}
RealExpression::RealExpression(value_type value) : value_(value) {}

Type* RealExpression::type() { return Type::getType(PrimitiveTypeEnum::REAL); }
void RealExpression::setType([[maybe_unused]] Type* type) {
  assert(false && "cannot set the type of an real literal");
}

RealExpression::value_type RealExpression::value() { return this->value_; }
} // namespace node
} // namespace ast
