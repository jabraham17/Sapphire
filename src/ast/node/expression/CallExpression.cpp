#include "CallExpression.h"
namespace ast {
namespace node {

Type* CallExpression::type() { return this->type_; }
void CallExpression::setType(Type* type) { this->type_ = type; }

OperatorType CallExpression::opType() { return this->op_; }
NodeList* CallExpression::operands() { return this->operands_; }

} // namespace node
} // namespace ast
