#ifndef SAPPHIRE_AST_NODE_EXPRESSION_CALLEXPRESSION_H_
#define SAPPHIRE_AST_NODE_EXPRESSION_CALLEXPRESSION_H_
#include "Expression.h"

#include "ast/ast.h"
#include "ast/node/NodeList.h"
#include "ast/node/OperatorType.h"
#include "ast/node/type/PrimitiveType.h"
#include "ast/node/type/Type.h"
#include "common/compiler-specific.h"

#include <type_traits>
namespace ast {
namespace node {
class CallExpression : public Expression {
  OperatorType op_;
  NodeList* operands_;
  Type* type_;

protected:
  virtual void replaceNode(ASTNode* old, ASTNode* replacement) override;

public:
  template <typename... Ts>
  CallExpression(long line, OperatorType opType, Ts&&... operands)
      : CallExpression(opType, std::forward<Ts>(operands)...) {
    setLine(line);
  }

  DIAGNOSTIC_PUSH
  // if there is only one template arg, this complains about an unused value
  DIAGNOSTIC_IGNORE("-Wunused-value")
  template <typename... Ts>
  CallExpression(OperatorType opType, Ts... operands)
      : op_(opType), operands_(new NodeList()), type_(Type::getUnknownType()) {
    static_assert(
        std::conjunction<std::bool_constant<(
            (std::is_pointer_v<Ts> &&
             std::is_base_of_v<ASTNode, std::remove_pointer_t<Ts>>),
            ...)>>::value,
        "");
    (([&] {
       if(auto nl = toNodeListNode(operands); nl != nullptr) {
         for(auto elm : *nl) {
           this->operands_->addBack(elm);
         }
       } else this->operands_->addBack(operands);
     }()),
     ...);
  }
  DIAGNOSTIC_POP
  virtual ~CallExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual ASTNode* clone() override;

  virtual Type* type() override;
  virtual void setType(Type* type) override;

  OperatorType opType();
  NodeList* operands();
};
} // namespace node
} // namespace ast
#endif
