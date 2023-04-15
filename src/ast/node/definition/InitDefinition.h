#ifndef SAPPHIRE_AST_NODE_INITDEFINITION_H_
#define SAPPHIRE_AST_NODE_INITDEFINITION_H_
#include "ast/ast.h"
#include "ast/node/ASTNode.h"
namespace ast {
namespace node {
class InitDefinition : public ASTNode {
private:
  NodeList* parameters_;
  Scope* body_;
  bool isInit_;

public:
  InitDefinition(long lineNumber, NodeList* parameters, Scope* body)
      : InitDefinition(parameters, body) {
    setLine(lineNumber);
  }
  InitDefinition(NodeList* parameters, Scope* body)
      : parameters_(parameters), body_(body), isInit_(true) {}
  InitDefinition(long lineNumber, Scope* body) : InitDefinition(body) {
    setLine(lineNumber);
  }
  InitDefinition(Scope* body)
      : parameters_(nullptr), body_(body), isInit_(false) {}
  virtual ~InitDefinition() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  bool isDeinit() { return !isInit_; }
  bool isInit() { return isInit_; }
  NodeList* parameters() { return parameters_; }
  Scope* body() { return body_; }
};

} // namespace node
} // namespace ast
#endif
