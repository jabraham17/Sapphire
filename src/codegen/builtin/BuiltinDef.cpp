#include "BuiltinDef.h"

#include "ast/nodes/type/nodes.h"

namespace codegen {
namespace builtin {

BuiltinDef* BuiltinDef::create(
    CodegenFunction codegenFunc,
    const std::string& name,
    ast::node::Type* returnType,
    const ast::node::ASTList& parameterTypes) {
  return new BuiltinDef(codegenFunc, name, returnType, parameterTypes);
}
BuiltinDef*
BuiltinDef::create(CodegenFunction codegenFunc, const std::string& name) {
  return create(codegenFunc, name, node::ast::Type::getNilType());
}
BuiltinDef* BuiltinDef::create(
    CodegenFunction codegenFunc,
    const std::string& name,
    ast::node::Type* returnType) {
  ast::node::ASTList params;
  return create(codegenFunc, name, returnType, params);
}
} // namespace builtin
static BuiltinDef* createOperator(
    CodegenFunction codegenFunc,
    ast::node::OperatorType operatorType,
    ast::node::Type* returnType,
    const ast::node::ASTList& parameterTypes) {
  return new BuiltinDef(codegenFunc, operatorType, returnType, parameterTypes);
}
} // namespace codegen
