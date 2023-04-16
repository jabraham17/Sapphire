#ifndef SAPPHIRE_AST_NODE_STATEMENT_SCOPE_H_
#define SAPPHIRE_AST_NODE_STATEMENT_SCOPE_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {
class Scope : public Statement {
  NodeList* statements_;

public:
  Scope(long lineNumber, NodeList* statements) : Scope(statements) {
    setLine(lineNumber);
  };
  Scope(NodeList* statements) : statements_(statements){};
  virtual ~Scope() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  NodeList* statements() { return statements_; }
};
} // namespace node
} // namespace ast
#endif
