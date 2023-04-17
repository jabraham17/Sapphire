#include "IntExpression.h"

#include "ast/node/type/PrimitiveType.h"
#include "ast/node/type/Type.h"

#include <cassert>

namespace ast {
namespace node {

ASTNode* IntExpression::clone() { return new IntExpression(value_); }
IntExpression::IntExpression(long line, value_type value)
    : IntExpression(value) {
  setLine(line);
}
IntExpression::IntExpression(value_type value) : value_(value) {}

Type* IntExpression::type() { return Type::getType(PrimitiveTypeEnum::INT); }
void IntExpression::setType([[maybe_unused]] Type* type) {
  assert(false && "cannot set the type of an int literal");
}

IntExpression::value_type IntExpression::value() { return this->value_; }
} // namespace node
} // namespace ast
