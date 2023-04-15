#ifndef SAPPHIRE_AST_NODE_TUPLETYPE_H_
#define SAPPHIRE_AST_NODE_TUPLETYPE_H_
#include "Type.h"
#include "TypeList.h"

#include "ast/ast.h"
namespace ast {
namespace node {
class TupleType : public Type {

private:
  TypeList* tupleTypes;

public:
  template <class InputIt>
  TupleType(long lineNumber, InputIt begin, InputIt end)
      : TupleType(begin, end) {
    setLine(lineNumber);
  }
  template <class InputIt>
  TupleType(InputIt begin, InputIt end) : TupleType(begin, end, false, false) {}
  template <class InputIt>
  TupleType(
      long lineNumber,
      InputIt begin,
      InputIt end,
      bool isRef,
      bool isNilable)
      : TupleType(begin, end, isRef, isNilable) {
    setLine(lineNumber);
  }
  template <class InputIt>
  TupleType(InputIt begin, InputIt end, bool isRef, bool isNilable)
      : Type(isRef, isNilable), tupleTypes(begin, end) {}
  virtual ~TupleType() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  TypeList::list elementTypes() { return tupleTypes->elementTypes; }
};
} // namespace node
} // namespace ast
#endif
