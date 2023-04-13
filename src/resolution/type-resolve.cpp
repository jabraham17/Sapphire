#include "type-resolve.h"

#include <cassert>
#include <iostream>
#include <unordered_map>

namespace pass {
namespace resolution {

class ResolveTypes final : public ast::visitor::VisitorWithArgsAndReturn<
                               ResolveTypes,
                               ast::visitor::VisitAll,
                               std::vector<std::string>> {
public:
  using VisitorWithArgsAndReturn::VisitorWithArgsAndReturn;

private:
  bool isIntegralType(ast::Type* type) {
    ast::PrimitiveType intType(ast::PrimitiveTypeEnum::INT);
    ast::PrimitiveType uintType(ast::PrimitiveTypeEnum::UINT);
    return ast::Type::isSameBaseType(type, &intType) ||
           ast::Type::isSameBaseType(type, &uintType);
  }

protected:
  virtual void visitImpl(ast::CallExpression* call) override {
    auto& errors = this->returnValue_;
    // resolve the operands
    call->operands()->accept(this);

    switch(call->op()->opType()) {
      case ast::OperatorType::ASSIGNMENT: {
        // if op0's type is unknown, set to be op1 type
        // op0 must equal op1
        auto op0 = ast::toExpressionNode(call->operands()->get(0));
        auto op1 = ast::toExpressionNode(call->operands()->get(1));
        assert(op0 != nullptr && op1 != nullptr);

        if(ast::Type::isUnknownType(op0->type())) {
          op0->setType(op1->type());
        } else if(ast::Type::isSameType(op0->type(), op1->type())) {
          // no need to do anything
        } else {
          errors.push_back(
              "type mismatch on '" + ast::Type::getTypeString(op0->type()) +
              " = " + ast::Type::getTypeString(op1->type()) + "' on line " +
              std::to_string(call->line()));
        }
        break;
      }
      case ast::OperatorType::SUBSCRIPT: {
        // TODO currently doesn't handle tuples

        // op0 must be an array, op1 must be an int or a uint
        // then we can safely say the result of this is the array element type
        auto op0 = ast::toExpressionNode(call->operands()->get(0));
        auto op1 = ast::toExpressionNode(call->operands()->get(1));
        assert(op0 != nullptr && op1 != nullptr);

        if(ast::Type::isArrayType(op0->type()) && isIntegralType(op1->type())) {
          auto elmType = ast::Type::toArrayType(op0->type())->elementType();
          // if the call type is unknown, we can set it
          if(ast::Type::isUnknownType(call->type())) {
            call->setType(elmType);
          } else {
            // user wrote a type mismatch
            errors.push_back(
                "type mismatch on '" + ast::Type::getTypeString(call->type()) +
                "' and '" + ast::Type::getTypeString(elmType) + "' on line " +
                std::to_string(call->line()));
          }
        } else {
          // type mismatch
          errors.push_back(
              "type mismatch on array access of '" +
              ast::Type::getTypeString(op0->type()) + "' with '" +
              ast::Type::getTypeString(op1->type()) + "' on line " +
              std::to_string(call->line()));
        }
        break;
      }
      default: break;
    }
  }
};

void TypeResolve::resolve() {
  {
    ResolveTypes rce;
    root->accept(&rce);
    auto errors = rce.returnValueAndClear();
    this->errors_.insert(this->errors_.end(), errors.begin(), errors.end());
  }
}
} // namespace resolution
} // namespace pass
