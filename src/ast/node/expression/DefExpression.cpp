#include "DefExpression.h"

#include "ast/node/Type.h"
#include "ast/symbol/symbol.h"
namespace ast {
namespace node {
DefExpression::DefExpression(
    Type* type,
    symbol::Symbol* symbol,
    Expression* assignValue)
    : symbol_(symbol), assignValue_(assignValue) {
  if(Type::isUnknownType(symbol_->type())) {
    symbol_->setType(type);
  }
}
DefExpression::DefExpression(Type* type, symbol::Symbol* symbol)
    : DefExpression(type, symbol, nullptr) {}
DefExpression::DefExpression(symbol::Symbol* symbol, Expression* assignValue)
    : DefExpression(assignValue->type(), symbol, assignValue) {}
DefExpression::DefExpression(symbol::Symbol* symbol)
    : DefExpression(symbol, nullptr) {}

bool DefExpression::hasInitialValue() {
  // auto n = toNilNode(assignValue_);
  return assignValue_ != nullptr;
  //  || (n != nullptr && n->isUserSpecified());
}
Type* DefExpression::type() { return symbol_->type(); }
void DefExpression::setType(Type* type) { symbol_->setType(type); }

} // namespace node
} // namespace ast
