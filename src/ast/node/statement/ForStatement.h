#ifndef SAPPHIRE_AST_NODE_STATEMENT_FORSTATEMENT_H_
#define SAPPHIRE_AST_NODE_STATEMENT_FORSTATEMENT_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {

class ForStatement : public Statement {
  DefExpression* var_;
  Expression* expr_;
  Scope* body_;

protected:
  virtual void replaceNode(ASTNode* old, ASTNode* replacement) override;

public:
  ForStatement(long line, DefExpression* var, Expression* expr, Scope* body);
  ForStatement(DefExpression* var, Expression* expr, Scope* body);
  virtual ~ForStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  DefExpression* variable();
  Expression* expr();
  Scope* body();
};
} // namespace node
} // namespace ast
#endif
