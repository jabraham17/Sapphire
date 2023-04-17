#include "InitDefinition.h"

#include "ast/node/NodeList.h"
#include "ast/node/statement/Scope.h"
namespace ast {
namespace node {
void InitDefinition::replaceNode(ASTNode* old, ASTNode* replacement) {
  if(parameters_ == old) {
    replacement->parent() = this;
    parameters_ =
        toNodeType<std::remove_pointer_t<decltype(parameters_)>>(replacement);
    return;
  }
  if(body_ == old) {
    replacement->parent() = this;
    body_ = toNodeType<std::remove_pointer_t<decltype(body_)>>(replacement);
    return;
  }
}

InitDefinition::InitDefinition(long line, NodeList* parameters, Scope* body)
    : InitDefinition(parameters, body) {
  setLine(line);
}
InitDefinition::InitDefinition(long line, Scope* body) : InitDefinition(body) {
  setLine(line);
}
InitDefinition::InitDefinition(NodeList* parameters, Scope* body)
    : parameters_(parameters), body_(body), isInit_(true) {
  parameters_->parent() = this;
  body_->parent() = this;
}
InitDefinition::InitDefinition(Scope* body)
    : parameters_(nullptr), body_(body), isInit_(false) {
  parameters_->parent() = this;
}

bool InitDefinition::isDeinit() { return !isInit_; }
bool InitDefinition::isInit() { return isInit_; }
NodeList* InitDefinition::parameters() { return parameters_; }
Scope* InitDefinition::body() { return body_; }
} // namespace node
} // namespace ast
