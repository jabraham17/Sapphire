#include "function-symbol.h"

#include "ast/node/definition/FunctionPrototype.h"
#include "ast/node/type/PrimitiveType.h"
#include "ast/node/type/Type.h"

namespace ast {

namespace symbol {

FunctionSymbol::FunctionSymbol(
    node::FunctionPrototype* prototype,
    const std::string& symbolName,
    node::Type* belongsTo,
    node::Type* type)
    : Symbol(symbolName, type), belongsTo_(belongsTo), prototype_(prototype) {}

FunctionSymbol::FunctionSymbol(
    node::FunctionPrototype* prototype,
    const char* symbolName,
    node::Type* belongsTo,
    node::Type* type)
    : Symbol(symbolName, type), belongsTo_(belongsTo), prototype_(prototype) {}

FunctionSymbol::FunctionSymbol(const std::string& symbolName, node::Type* type)
    : FunctionSymbol(nullptr, symbolName, node::Type::getUntypedType(), type) {}

FunctionSymbol::FunctionSymbol(const char* symbolName, node::Type* type)
    : FunctionSymbol(nullptr, symbolName, node::Type::getUntypedType(), type) {}

std::string FunctionSymbol::name() {
  std::string s;
  if(prototype()->isMangled()) s += "spp_";
  if(!this->belongsTo()->isUntypedType() && !this->belongsTo()->isUnknownType())
    s += belongsTo()->toMangledString() + "_";
  s += Symbol::name();
  return s;
}

std::string FunctionSymbol::toString(bool typed, bool showFlags) {
  std::string s;
  if(!this->belongsTo()->isUntypedType() &&
     !this->belongsTo()->isUnknownType()) {
    s += this->belongsTo()->toString() + ".";
  }
  s += Symbol::toString(typed, showFlags);
  return s;
}

node::FunctionPrototype* FunctionSymbol::prototype() {
  return this->prototype_;
};
void FunctionSymbol::setPrototype(node::FunctionPrototype* prototype) {
  this->prototype_ = prototype;
}

node::Type* FunctionSymbol::belongsTo() { return this->belongsTo_; }
void FunctionSymbol::setBelongsTo(node::Type* belongsTo) {
  this->belongsTo_ = belongsTo;
}

bool FunctionSymbol::operator==(FunctionSymbol& other) {
  return this->basename() == other.basename() &&
         node::Type::isSameType(this->type(), other.type()) &&
         node::Type::isSameType(this->belongsTo(), other.belongsTo());
}

} // namespace symbol
} // namespace ast
