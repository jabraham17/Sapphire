#include "Parameter.h"

#include "ast/symbol/symbol.h"
namespace ast {
namespace node {
Parameter::Parameter(long line, symbol::Symbol* symbol, Type* type)
    : Parameter(symbol, type) {
  setLine(line);
}
Parameter::Parameter(symbol::Symbol* symbol, Type* type) : symbol_(symbol) {
  symbol_->setType(type);
}

symbol::Symbol* Parameter::symbol() { return this->symbol_; }

} // namespace node
} // namespace ast
