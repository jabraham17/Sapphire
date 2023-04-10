#ifndef SAPPHIRE_AST_VISITORS_VISIT_ALL_H_
#define SAPPHIRE_AST_VISITORS_VISIT_ALL_H_
#include "ast/ast-visitor.h"
#include "ast/ast.h"

namespace ast {
namespace visitor {

class VisitAll : public ASTVisitor {

public:
  VisitAll() = default;
  virtual ~VisitAll() = default;

protected:
#define ast_node_def_exclude
#define ast_node_def_exclude_Parameter
#define ast_node_def_exclude_Statement
#define ast_node_def_exclude_Type
#define ast_node_def_exclude_PrimitiveType
#define ast_node_def_exclude_ClassType
#define ast_node_def_exclude_UseExpression
#define ast_node_def_exclude_Operator
#define ast_node_def_exclude_IntExpression
#define ast_node_def_exclude_UIntExpression
#define ast_node_def_exclude_RealExpression
#define ast_node_def_exclude_StringExpression
#define ast_node_def_exclude_Nil
#define ast_node_def(type) virtual void visit##type##Impl(type*) override;
#include "ast/ast-node.inc"
};
} // namespace visitor
} // namespace ast

#endif
