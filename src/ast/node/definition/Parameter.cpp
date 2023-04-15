#include "Parameter.h"
namespace ast {
namespace node {
Parameter::Parameter(symbol::Symbol* symbol, Type* type) : symbol_(symbol) {
  symbol_->setType(type);
}
} // namespace node
} // namespace ast
