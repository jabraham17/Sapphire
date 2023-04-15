#include "UseExpression.h"

#include "ast/symbol/symbol.h"
namespace ast {
namespace node {
Type* UseExpression::type() { return symbol_->type(); }
void UseExpression::setType(Type* type) { symbol_->setType(type); }
} // namespace node
} // namespace ast
