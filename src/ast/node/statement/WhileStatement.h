#ifndef SAPPHIRE_AST_NODE_STATEMENT_WHILESTATEMENT_H_
#define SAPPHIRE_AST_NODE_STATEMENT_WHILESTATEMENT_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {

// todo: move to cpp definition stuff

class WhileStatement : public Statement {
  Expression* expr_;
  Scope* body_;

protected:
  virtual void replaceNode(ASTNode* old, ASTNode* replacement) override;

public:
  WhileStatement(long line, Expression* expr, Scope* body);
  WhileStatement(Expression* expr, Scope* body);
  virtual ~WhileStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  Expression* expr();
  Scope* body();
};
} // namespace node
} // namespace ast
#endif
