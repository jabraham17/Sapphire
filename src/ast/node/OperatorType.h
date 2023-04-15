#ifndef SAPPHIRE_AST_NODE_OPERATORTYPE_H_
#define SAPPHIRE_AST_NODE_OPERATORTYPE_H_

#include "ast/ast.h"

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
[[maybe_unused]] static std::string getOperatorTypeString(OperatorType ot) {
  switch(ot) {
    case OperatorType::ASSIGNMENT: return "=";
    case OperatorType::CONDITIONAL: return "?:";
    case OperatorType::NEW_CLASS: return "newClass";
    case OperatorType::NEW_ARRAY: return "newArray";
    case OperatorType::NEW_TUPLE: return "newTuple";
    case OperatorType::FUNCTION: return "call";
    case OperatorType::FORCE_UNWRAP: return "unwrap";
    case OperatorType::OPTIONAL_UNWRAP: return "unwrap?";
    case OperatorType::TAKE_REF: return "getRef";
    case OperatorType::FIELD_ACCESS: return ".";
    case OperatorType::SUBSCRIPT: return "[]";
    case OperatorType::PLUS: return "+";
    case OperatorType::MINUS: return "-";
    case OperatorType::MULTIPLY: return "*";
    case OperatorType::DIVIDE: return "/";
    case OperatorType::LSHIFT: return "<<";
    case OperatorType::RSHIFT: return ">>";
    case OperatorType::MODULO: return "&";
    case OperatorType::AND: return "&&";
    case OperatorType::OR: return "&";
    case OperatorType::BW_AND: return "&";
    case OperatorType::BW_OR: return "|";
    case OperatorType::EQ: return "==";
    case OperatorType::NEQ: return "!=";
    case OperatorType::LT: return "<";
    case OperatorType::GT: return ">";
    case OperatorType::LTEQ: return "<=";
    case OperatorType::GTEQ: return ">=";
    case OperatorType::BW_NEGATE: return "~";
    case OperatorType::NEGATE: return "-";
    case OperatorType::NOT: return "!";
  }
}

} // namespace node
} // namespace ast
#endif
