#ifndef SAPPHIRE_AST_NODE_STATEMENT_WHILESTATEMENT_H_
#define SAPPHIRE_AST_NODE_STATEMENT_WHILESTATEMENT_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {

// todo: move to cpp definition stuff

class WhileStatement : public Statement {
  std::size_t exprIdx_;
  std::size_t bodyIdx_;

public:
  WhileStatement(long line, Expression* expr, Scope* body);
  WhileStatement(Expression* expr, Scope* body);
  virtual ~WhileStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  // virtual ASTNode* clone() override;

  CONST_MEMBER_FUNC(Expression*, expr)
  CONST_MEMBER_FUNC(Scope*, body)
};
} // namespace node
} // namespace ast
#endif
