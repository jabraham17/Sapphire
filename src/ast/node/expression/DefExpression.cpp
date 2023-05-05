#include "DefExpression.h"

#include "ast/node/type/Type.h"
#include "ast/symbol/symbol.h"
namespace ast {
namespace node {

// TODO: clone the symbol
// ASTNode* DefExpression::clone() {
//   return new DefExpression(
//       toNodeType<std::remove_pointer_t<decltype(type())>>(type()->clone()),
//       symbol_,
//       initialValue_
//           ? toNodeType<std::remove_pointer_t<decltype(initialValue_)>>(
//                 initialValue_->clone())
//           : nullptr);
// }

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
    : symbol_(symbol), initialValueIdx_(-1) {
  if(symbol_->type()->isUnknownType()) {
    symbol_->setType(type);
  }
  if(initialValue) setInitialValue(initialValue);
}
DefExpression::DefExpression(symbol::Symbol* symbol, Expression* initialValue)
    : DefExpression((Type*)Type::getUnknownType(), symbol, initialValue) {}

symbol::Symbol* DefExpression::symbol() { return this->symbol_; }

Type* DefExpression::type() { return this->symbol()->type(); }
void DefExpression::setType(Type* type) { this->symbol()->setType(type); }

void DefExpression::setInitialValue(Expression* value) {
  if(hasInitialValue()) {
    auto old = child(this->initialValueIdx_);
    setChild(this->initialValueIdx_, value);
    delete old;
  } else {
    this->initialValueIdx_ = addChild(value);
  }
}
bool DefExpression::hasInitialValue() { return this->initialValueIdx_ != -1; }
Expression* DefExpression::initialValue() {
  if(this->initialValueIdx_)
    return child(this->initialValueIdx_)->toExpression();
  else return nullptr;
}

} // namespace node
} // namespace ast
