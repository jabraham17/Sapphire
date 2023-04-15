#include "StringExpression.h"
namespace ast {
namespace node {

char codeForEscapedChar(char c) {
  if(c == 'n') return '\n';
  if(c == 't') return '\t';
  if(c == '\\') return '\\';
  return c;
}

std::string StringExpression::escapedValue() {
  std::string v = str;
  size_t pos = 0;
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
