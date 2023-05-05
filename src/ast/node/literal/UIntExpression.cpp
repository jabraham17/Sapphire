#include "UIntExpression.h"

#include "ast/node/type/PrimitiveType.h"
#include "ast/node/type/Type.h"

#include <cassert>

namespace ast {
namespace node {

// ASTNode* UIntExpression::clone() { return new UIntExpression(value_); }

UIntExpression::UIntExpression(long line, value_type value)
    : UIntExpression(value) {
  setLine(line);
}
UIntExpression::UIntExpression(value_type value) : value_(value) {}

Type* UIntExpression::type() { return Type::getType(PrimitiveTypeEnum::UINT); }
void UIntExpression::setType([[maybe_unused]] Type* type) {
  assert(false && "cannot set the type of an uint literal");
}

UIntExpression::value_type UIntExpression::value() { return this->value_; }

} // namespace node
} // namespace ast
