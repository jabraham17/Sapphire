#include "ClassType.h"
namespace ast {
namespace node {

ASTNode* ClassType::clone() {
  auto x = new ClassType(className_.c_str(), this->isRef(), this->isNilable());
  return x;
}
} // namespace node
} // namespace ast
