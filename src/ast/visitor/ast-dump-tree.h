#ifndef SAPPHIRE_AST_VISITORS_AST_DUMP_TREE_H_
#define SAPPHIRE_AST_VISITORS_AST_DUMP_TREE_H_

#include "ast/ast.h"
#include "common/indented-ostream.h"

#include <iostream>

#include "ast-visitor.h"

namespace ast {

namespace visitor {

class ASTDumpTree : public ASTVisitor {
private:
  common::IndentedOStream strm;

public:
  ASTDumpTree(std::ostream& rawStrm, size_t indentLevel = 2)
      : strm(rawStrm, indentLevel) {}
  virtual ~ASTDumpTree() = default;

protected:
#define ast_node_def(type) virtual void visitImpl(node::type*) override;
#include "ast/ast-node.inc"
};
} // namespace visitor
} // namespace ast

#endif
