#include "Parameter.h"

#include "ast/symbol/symbol.h"
namespace ast {
namespace node {
Parameter::Parameter(long lineNumber, symbol::Symbol* symbol, Type* type)
    : Parameter(symbol, type) {
  setLine(lineNumber);
}
Parameter::Parameter(symbol::Symbol* symbol, Type* type) : symbol_(symbol) {
  symbol_->setType(type);
}

symbol::Symbol* Parameter::symbol() { return this->symbol_; }

} // namespace node
} // namespace ast
