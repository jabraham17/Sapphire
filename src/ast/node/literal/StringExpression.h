#ifndef SAPPHIRE_AST_NODE_STRINGEXPRESSION_H_
#define SAPPHIRE_AST_NODE_STRINGEXPRESSION_H_
#include "ast/ast.h"
#include "ast/node/expression/Expression.h"
namespace ast {
namespace node {

class StringExpression : public Expression {
private:
  std::string str;

public:
  StringExpression(long lineNumber, const std::string& value)
      : StringExpression(value) {
    setLine(lineNumber);
  }
  StringExpression(const std::string str) : str(str) {}
  virtual ~StringExpression() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  virtual Type* type() override {
    return new PrimitiveType(PrimitiveTypeEnum::STRING);
  }
  virtual void setType(Type* type) override {
    assert(false && "cannot set the type of a string expression");
  }
  std::string value() { return str; }
  //  return the string with all escapes resolved
  std::string escapedValue();
};

} // namespace node
} // namespace ast
#endif
