#ifndef SAPPHIRE_AST_NODE_EXPRESSION_CALLEXPRESSION_H_
#define SAPPHIRE_AST_NODE_EXPRESSION_CALLEXPRESSION_H_
#include "Expression.h"

#include "ast/ast.h"
#include "ast/node/NodeList.h"
#include "ast/node/OperatorType.h"
#include "ast/node/type/PrimitiveType.h"
#include "ast/node/type/Type.h"
namespace ast {
namespace node {
class CallExpression : public Expression {
  OperatorType op_;
  NodeList* operands_;
  Type* type_;

public:
  template <typename... Args>
  CallExpression(long lineNumber, OperatorType opType, Args... operands)
      : CallExpression(opType, operands...) {
    setLine(lineNumber);
  }
  template <typename... Args>
  CallExpression(OperatorType opType, Args... operands)
      : op_(opType), operands_(new NodeList()), type_(Type::getUnknownType()) {
    initOperands(std::forward<Args>(operands)...);
  }
  virtual ~CallExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual Type* type() override { return type_; }
  virtual void setType(Type* type) override { this->type_ = type; }

private:
  template <typename Arg> void initOperands(Arg operand) {
    // unwrap node lists past in
    if(auto nl = toNodeListNode(operand); nl != nullptr) {
      for(auto elm : *nl) {
        this->operands_->addBack(elm);
      }
    } else this->operands_->addBack(operand);
  }
  template <typename Arg, typename... Args>
  void initOperands(Arg operand, Args... operands) {
    initOperands(std::forward<Arg>(operand));
    initOperands(std::forward<Args>(operands)...);
  }

public:
  OperatorType opType() { return op_; }
  NodeList* operands() { return operands_; }
};
} // namespace node
} // namespace ast
#endif
