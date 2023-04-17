#ifndef SAPPHIRE_AST_NODE_STATEMENT_RETURNSTATEMENT_H_
#define SAPPHIRE_AST_NODE_STATEMENT_RETURNSTATEMENT_H_
#include "Statement.h"

#include "ast/ast.h"
namespace ast {
namespace node {
// todo: move to cpp definition stuff

class ReturnStatement : public Statement {
  Expression* expr_;

protected:
  virtual void replaceNode(ASTNode* old, ASTNode* replacement) override;

public:
  ReturnStatement(long line, Expression* expr);
  ReturnStatement(Expression* expr);
  virtual ~ReturnStatement() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual ASTNode* clone() override;

  Expression* expression();
};
} // namespace node
} // namespace ast
#endif
