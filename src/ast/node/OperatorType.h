#ifndef SAPPHIRE_AST_NODE_OPERATORTYPE_H_
#define SAPPHIRE_AST_NODE_OPERATORTYPE_H_

#include "ast/ast.h"

#include <string>

namespace ast {
namespace node {
enum class OperatorType {
  ASSIGNMENT,
  CONDITIONAL,
  NEW_CLASS,
  NEW_ARRAY,
  NEW_TUPLE,
  FUNCTION,
  FORCE_UNWRAP,
  OPTIONAL_UNWRAP,
  TAKE_REF,
  FIELD_ACCESS,
  SUBSCRIPT,
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
  LSHIFT,
  RSHIFT,
  MODULO,
  AND,
  OR,
  BW_AND,
  BW_OR,
  EQ,
  NEQ,
  LT,
  GT,
  LTEQ,
  GTEQ,
  BW_NEGATE,
  NEGATE,
  NOT
};
std::string getOperatorTypeString(OperatorType ot);

} // namespace node
} // namespace ast
#endif
