#ifndef SAPPHIRE_AST_NODE_STATEMENT_FORSTATEMENT_H_
#define SAPPHIRE_AST_NODE_STATEMENT_FORSTATEMENT_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {

class ForStatement : public Statement {
  std::size_t varIdx_;
  std::size_t exprIdx_;
  std::size_t bodyIdx_;

public:
  ForStatement(long line, DefExpression* var, Expression* expr, Scope* body);
  ForStatement(DefExpression* var, Expression* expr, Scope* body);
  virtual ~ForStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  // virtual ASTNode* clone() override;

  CONST_MEMBER_FUNC(DefExpression*, variable)
  CONST_MEMBER_FUNC(Expression*, expr)
  CONST_MEMBER_FUNC(Scope*, body)
};
} // namespace node
} // namespace ast
#endif
