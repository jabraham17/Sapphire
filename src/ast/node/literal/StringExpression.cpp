#include "StringExpression.h"

#include "ast/node/type/PrimitiveType.h"
#include "ast/node/type/Type.h"

#include <cassert>

namespace ast {
namespace node {

StringExpression::StringExpression(long lineNumber, const value_type& value)
    : StringExpression(value) {
  setLine(lineNumber);
}
StringExpression::StringExpression(const value_type& value) : value_(value) {}

Type* StringExpression::type() {
  return Type::getType(PrimitiveTypeEnum::STRING);
}
void StringExpression::setType([[maybe_unused]] Type* type) {
  assert(false && "cannot set the type of an string literal");
}

StringExpression::value_type StringExpression::value() { return this->value_; }

char codeForEscapedChar(char c) {
  if(c == 'n') return '\n';
  if(c == 't') return '\t';
  if(c == '\\') return '\\';
  return c;
}

std::string StringExpression::escapedValue() {
  std::string v = this->value_;
  std::size_t pos = 0;
  while(true) {
    pos = v.find('\\');
    if(pos == std::string::npos) break;
    char replacementChar = codeForEscapedChar(v[pos + 1]);
    std::string replacement;
    replacement += replacementChar;
    v.replace(pos, 2, replacement);
  }
  return v;
}

} // namespace node
} // namespace ast
