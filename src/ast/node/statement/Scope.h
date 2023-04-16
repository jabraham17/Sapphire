#ifndef SAPPHIRE_AST_NODE_STATEMENT_SCOPE_H_
#define SAPPHIRE_AST_NODE_STATEMENT_SCOPE_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {

// todo: move to cpp definition stuff

class Scope : public Statement {
  NodeList* statements_;

public:
  Scope(long line, NodeList* statements) : Scope(statements) { setLine(line); };
  Scope(NodeList* statements) : statements_(statements){};
  virtual ~Scope() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  NodeList* statements() { return statements_; }
};
} // namespace node
} // namespace ast
#endif
