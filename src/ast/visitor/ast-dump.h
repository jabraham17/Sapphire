#ifndef SAPPHIRE_AST_VISITOR_AST_DUMP_H_
#define SAPPHIRE_AST_VISITOR_AST_DUMP_H_

#include "ast/ast.h"
#include "common/indented-ostream.h"

#include <iostream>

#include "ast-visitor.h"

namespace ast {

namespace visitor {

class ASTDump : public ASTVisitor {
private:
  common::IndentedOStream strm;

public:
  ASTDump(std::ostream& rawStrm, std::size_t indentLevel = 2)
      : strm(rawStrm, indentLevel) {}
  virtual ~ASTDump() = default;

private:
protected:
#define ast_node_def_exclude
#define ast_node_def_exclude_Statement
#define ast_node_def_exclude_Expression
#define ast_node_def_exclude_Type
#define ast_node_def(type) virtual void visitImpl(node::type* arg) override;
#include "ast/ast-node.inc"
};
} // namespace visitor
} // namespace ast

#endif
