#include "type-resolve.h"

#include "ast/node/nodes.h"

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
  bool isIntegralType(ast::node::Type* type) {
    auto intType = ast::node::Type::getType(ast::node::PrimitiveTypeEnum::INT);
    auto uintType =
        ast::node::Type::getType(ast::node::PrimitiveTypeEnum::UINT);
    return ast::node::Type::isSameBaseType(type, intType) ||
           ast::node::Type::isSameBaseType(type, uintType);
  }
  bool isStringType(ast::node::Type* type) {
    auto stringType =
        ast::node::Type::getType(ast::node::PrimitiveTypeEnum::STRING);
    return ast::node::Type::isSameBaseType(type, stringType);
  }
  bool isNumType(ast::node::Type* type) {
    auto realType =
        ast::node::Type::getType(ast::node::PrimitiveTypeEnum::REAL);
    auto byteType =
        ast::node::Type::getType(ast::node::PrimitiveTypeEnum::BYTE);
    return isIntegralType(type) ||
           ast::node::Type::isSameBaseType(type, realType) ||
           ast::node::Type::isSameBaseType(type, byteType);
  }

protected:
  virtual void visitImpl(ast::node::CallExpression* call) override {
    auto& errors = this->returnValue_;
    // resolve the operands
    call->operands()->accept(this);

    switch(call->opType()) {
      case ast::node::OperatorType::PLUS: {
        // if num types, string types, or array types, OF THE SAME TYPE, return
        // the same type
        auto op0 = ast::toExpressionNode(call->operands()->get(0));
        auto op1 = ast::toExpressionNode(call->operands()->get(1));
        assert(op0 != nullptr && op1 != nullptr);

        if(ast::node::Type::isSameType(op0->type(), op1->type()) &&
           (isNumType(op0->type()) || isStringType(op0->type()) ||
            op0->type()->isArrayType())) {
          call->setType(op0->type());
        } else {
          errors.push_back(
              "type mismatch on '" + op0->type()->toString() + " + " +
              op1->type()->toString() + "' on line " +
              std::to_string(call->line()));
        }
        break;
      }
      case ast::node::OperatorType::ASSIGNMENT: {
        // if op0's type is unknown, set to be op1 type
        // op0 must equal op1
        auto op0 = ast::toExpressionNode(call->operands()->get(0));
        auto op1 = ast::toExpressionNode(call->operands()->get(1));
        assert(op0 != nullptr && op1 != nullptr);

        if(op0->type()->isUnknownType()) {
          op0->setType(op1->type());
          call->setType(op0->type());
        } else if(ast::node::Type::isSameType(op0->type(), op1->type())) {
          // no need to do anything
        } else {
          errors.push_back(
              "type mismatch on '" + op0->type()->toString() + " = " +
              op1->type()->toString() + "' on line " +
              std::to_string(call->line()));
        }
        break;
      }
      case ast::node::OperatorType::SUBSCRIPT: {
        // TODO currently doesn't handle tuples

        // op0 must be an array, op1 must be an int or a uint
        // then we can safely say the result of this is the array element type
        auto op0 = ast::toExpressionNode(call->operands()->get(0));
        auto op1 = ast::toExpressionNode(call->operands()->get(1));
        assert(op0 != nullptr && op1 != nullptr);

        if(op0->type()->isArrayType() && isIntegralType(op1->type())) {
          auto elmType = op0->type()->toArrayType()->elementType();
          // if the call type is unknown, we can set it
          if(call->type()->isUnknownType()) {
            call->setType(elmType);
          } else {
            // user wrote a type mismatch
            errors.push_back(
                "type mismatch on '" + call->type()->toString() + "' and '" +
                elmType->toString() + "' on line " +
                std::to_string(call->line()));
          }
        } else {
          // type mismatch
          errors.push_back(
              "type mismatch on array access of '" + op0->type()->toString() +
              "' with '" + op1->type()->toString() + "' on line " +
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
