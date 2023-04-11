#ifndef SAPPHIRE_RESOLUTION_TYPE_RESOLUTION_H_
#define SAPPHIRE_RESOLUTION_TYPE_RESOLUTION_H_

#include "ast/ast.h"
#include "ast/visitors/ast-visitor.h"

namespace pass {
namespace resolution {

static ast::Symbol* getSymbol(ast::ASTNode* ast) {
  if(auto a = ast::toUseExpressionNode(ast); a != nullptr) return a->symbol();
  return nullptr;
}

class TypeResolutionPass : public ast::visitor::VisitAll {
public:
  TypeResolutionPass() = default;
  virtual ~TypeResolutionPass() = default;

protected:
  virtual void visitImpl(ast::CallExpression* arg) override {
    std::cout << "call\n";
    switch(arg->op()->opType()) {
      case ast::OperatorType::SUBSCRIPT: {
        std::cout << "sub\n";
#warning this is really really hacky, i need to write some proper scope resolution
        // TODO really hacky, actually should be apart of scope resolution but
        // this is fine :) operand 0 must be an array
        auto sym = getSymbol(arg->operands()->get(0));
        if(sym) {
          std::cout << "sym\n";
          sym->setType(new ast::ArrayType(
              new ast::PrimitiveType(ast::PrimitiveTypeEnum::STRING)));
        }
        break;
      }
      default: {
        for(auto a : *arg->operands()) {
          a->accept(this);
        }
      }
    }
  }
};

} // namespace resolution
} // namespace pass

#endif
