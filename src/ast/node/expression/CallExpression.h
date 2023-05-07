#ifndef SAPPHIRE_AST_NODE_EXPRESSION_CALLEXPRESSION_H_
#define SAPPHIRE_AST_NODE_EXPRESSION_CALLEXPRESSION_H_
#include "Expression.h"

#include "ast/ast.h"
#include "ast/node/OperatorType.h"
#include "ast/node/type/PrimitiveType.h"
#include "ast/node/type/Type.h"
#include "common/compiler-specific.h"

#include <type_traits>

template <typename> struct is_std_vector : std::false_type {};

template <typename T> struct is_std_vector<std::vector<T>> : std::true_type {};

namespace ast {
namespace node {
class CallExpression : public Expression {
  OperatorType op_;
  std::size_t operandsStartIdx_;
  std::size_t numOperands;
  std::size_t typeIdx_;

public:
  template <typename... Ts>
  CallExpression(long line, OperatorType opType, Ts&&... operands)
      : CallExpression(opType, std::forward<Ts>(operands)...) {
    setLine(line);
  }
  CallExpression(long line, OperatorType opType) : CallExpression(opType) {
    setLine(line);
  }

  DIAGNOSTIC_PUSH
  // if there is only one template arg, this complains about an unused value
  DIAGNOSTIC_IGNORE("-Wunused-value")
  template <typename... Ts>
  CallExpression(OperatorType opType, Ts... operands) : CallExpression(opType) {
    static_assert(
        std::conjunction<std::bool_constant<(
            (std::is_pointer_v<Ts> &&
             std::is_base_of_v<ASTNode, std::remove_pointer_t<Ts>>),
            ...)>>::value,
        "");
    this->operandsStartIdx_ = this->numChildren();
    auto nOperands = 0;
    (([&] {
       this->addChild(operands);
       nOperands++;
     }()),
     ...);
    // operands should be the list children in list
    this->numOperands = nOperands;
  }
  DIAGNOSTIC_POP
  CallExpression(OperatorType opType) : op_(opType) {
    this->typeIdx_ = this->addChild(Type::getUnknownType());
    this->operandsStartIdx_ = this->numChildren();
    this->numOperands = 0;
  }
  virtual ~CallExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  // virtual ASTNode* clone() override;

  virtual Type* type() override;
  virtual void setType(Type* type) override;

  void addOperand(ASTNode* node);
  void addOperands(const ASTList& nodes);

  CONST_MEMBER_FUNC(OperatorType, opType);
  CONST_MEMBER_FUNC(ASTListIteratorPair<ASTNode>, operands);
};
} // namespace node
} // namespace ast
#endif
