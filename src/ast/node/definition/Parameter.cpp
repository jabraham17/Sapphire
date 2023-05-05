#include "Parameter.h"

#include "ast/node/type/Type.h"
#include "ast/symbol/symbol.h"
namespace ast {
namespace node {

// // todo: clone the symbol
// ASTNode* Parameter::clone() {
//   return new Parameter(
//       symbol_,
//       toNodeType<std::remove_pointer_t<decltype(symbol_->type())>>(
//           symbol_->type()->clone()));
// }
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
