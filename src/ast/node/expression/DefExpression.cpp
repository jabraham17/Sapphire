#include "DefExpression.h"

#include "ast/node/type/Type.h"
#include "ast/symbol/symbol.h"
namespace ast {
namespace node {

DefExpression::DefExpression(
    long line,
    Type* type,
    symbol::Symbol* symbol,
    Expression* initialValue)
    : DefExpression(type, symbol, initialValue) {
  setLine(line);
}
DefExpression::DefExpression(
    long line,
    symbol::Symbol* symbol,
    Expression* initialValue)
    : DefExpression(symbol, initialValue) {
  setLine(line);
}
DefExpression::DefExpression(
    Type* type,
    symbol::Symbol* symbol,
    Expression* initialValue)
    : symbol_(symbol), initialValue_(initialValue) {
  if(symbol_->type()->isUnknownType()) {
    symbol_->setType(type);
  }
}
DefExpression::DefExpression(symbol::Symbol* symbol, Expression* initialValue)
    : DefExpression((Type*)Type::getUnknownType(), symbol, initialValue) {}

symbol::Symbol* DefExpression::symbol() { return this->symbol_; }

Type* DefExpression::type() { return this->symbol()->type(); }
void DefExpression::setType(Type* type) { this->symbol()->setType(type); }

void DefExpression::setInitialValue(Expression* value) {
  this->initialValue_ = value;
}
bool DefExpression::hasInitialValue() { return this->initialValue_ != nullptr; }
Expression* DefExpression::initialValue() { return this->initialValue_; }

} // namespace node
} // namespace ast
