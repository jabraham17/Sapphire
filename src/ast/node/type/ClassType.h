#ifndef SAPPHIRE_AST_NODE_TYPE_CLASSTYPE_H_
#define SAPPHIRE_AST_NODE_TYPE_CLASSTYPE_H_
#include "Type.h"

#include "ast/ast.h"
namespace ast {
namespace node {
// todo: move to cpp definition stuff
class ClassType : public Type {

private:
  std::string className_;

public:
  ClassType(long line, const char* className) : ClassType(className) {
    setLine(line);
  }
  ClassType(const char* className) : ClassType(className, false, false) {}
  ClassType(long line, const char* className, bool isRef, bool isNilable)
      : ClassType(className, isRef, isNilable) {
    setLine(line);
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
