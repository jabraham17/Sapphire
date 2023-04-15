#ifndef SAPPHIRE_AST_NODE_TYPE_CLASSTYPE_H_
#define SAPPHIRE_AST_NODE_TYPE_CLASSTYPE_H_
#include "Type.h"

#include "ast/ast.h"
namespace ast {
namespace node {
class ClassType : public Type {

private:
  std::string className_;

public:
  ClassType(long lineNumber, const char* className) : ClassType(className) {
    setLine(lineNumber);
  }
  ClassType(const char* className) : ClassType(className, false, false) {}
  ClassType(long lineNumber, const char* className, bool isRef, bool isNilable)
      : ClassType(className, isRef, isNilable) {
    setLine(lineNumber);
  }
  ClassType(const char* className, bool isRef, bool isNilable)
      : Type(isRef, isNilable), className_(className) {}

  virtual ~ClassType() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  std::string className() { return className_; }
};
} // namespace node
} // namespace ast
#endif
