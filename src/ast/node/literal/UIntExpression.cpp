#include "UIntExpression.h"

#include "ast/node/Type.h"
namespace ast {
namespace node {

UIntExpression::UIntExpression(long lineNumber, value_type value)
    : UIntExpression(value) {
  setLine(lineNumber);
}
UIntExpression::UIntExpression(value_type value) : value_(value) {}

Type* UIntExpression::type() {
  return new PrimitiveType(PrimitiveTypeEnum::UINT);
}
void UIntExpression::setType(Type* type) {
  assert(false && "cannot set the type of an uint expression");
}

UIntExpression::value_type value() { return value_; }

} // namespace node
} // namespace ast
