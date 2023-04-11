#ifndef SAPPHIRE_RESOLUTION_TYPE_RESOLVE_H_
#define SAPPHIRE_RESOLUTION_TYPE_RESOLVE_H_

#include "ast/ast.h"
#include "ast/visitors/ast-visitor.h"

namespace pass {
namespace resolution {

static ast::Symbol* getSymbol(ast::ASTNode* ast) {
  if(auto a = ast::toUseExpressionNode(ast); a != nullptr) return a->symbol();
  return nullptr;
}

class TypeResolve : public ast::visitor::VisitAll {
public:
  TypeResolve() = default;
  virtual ~TypeResolve() = default;

protected:
  virtual void visitImpl(ast::CallExpression* arg) override {
    switch(arg->op()->opType()) {
      case ast::OperatorType::SUBSCRIPT: {
#warning this is really really hacky, i need to write some proper scope resolution
        // TODO really hacky, actually should be apart of scope resolution but
        // this is fine :) operand 0 must be an array
        auto sym = getSymbol(arg->operands()->get(0));
        if(sym) {
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
