#include "UseExpression.h"

#include "ast/symbol/symbol.h"
namespace ast {
namespace node {

// TODO: symbol is not being cloned
// ASTNode* UseExpression::clone() { return new UseExpression(symbol_); }
UseExpression::UseExpression(long line, symbol::Symbol* symbol)
    : UseExpression(symbol) {
  setLine(line);
}
UseExpression::UseExpression(symbol::Symbol* symbol) : symbol_(symbol) {}

Type* UseExpression::type() { return this->symbol()->type(); }
symbol::Symbol* UseExpression::symbol() { return this->symbol_; }

void UseExpression::setType(Type* type) { this->symbol()->setType(type); }
void UseExpression::setSymbol(symbol::Symbol* symbol) {
  this->symbol_ = symbol;
}
} // namespace node
} // namespace ast
