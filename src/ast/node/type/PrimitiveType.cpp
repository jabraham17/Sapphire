#include "PrimitiveType.h"
namespace ast {
namespace node {

ASTNode* PrimitiveType::clone() {
  auto x = new PrimitiveType(primType, this->isRef(), this->isNilable());
  return x;
}
} // namespace node
} // namespace ast
