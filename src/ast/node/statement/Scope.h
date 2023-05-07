#ifndef SAPPHIRE_AST_NODE_STATEMENT_SCOPE_H_
#define SAPPHIRE_AST_NODE_STATEMENT_SCOPE_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {

// todo: move to cpp definition stuff

class Scope : public Statement {
  std::size_t statementsStartIdx_;
  std::size_t numStatements_;

public:
  Scope(long line, const ASTList& statements);
  Scope(const ASTList& statements);
  Scope(long line);
  Scope();
  virtual ~Scope() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  // virtual ASTNode* clone() override;

  CONST_MEMBER_FUNC(ASTListIteratorPair<Statement>, statements);
};
} // namespace node
} // namespace ast
#endif
