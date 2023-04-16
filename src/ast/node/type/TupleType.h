#ifndef SAPPHIRE_AST_NODE_TYPE_TUPLETYPE_H_
#define SAPPHIRE_AST_NODE_TYPE_TUPLETYPE_H_
#include "Type.h"
#include "TypeList.h"

#include "ast/ast.h"
namespace ast {
namespace node {
// todo: move to cpp definition stuff
// todo: cleanup templates to be consistent
class TupleType : public Type {

private:
  TypeList* tupleTypes;

public:
  template <class InputIt>
  TupleType(long line, InputIt begin, InputIt end) : TupleType(begin, end) {
    setLine(line);
  }
  template <class InputIt>
  TupleType(InputIt begin, InputIt end) : TupleType(begin, end, false, false) {}

  TupleType(TypeList* tupleTypes) : TupleType(tupleTypes, false, false) {}
  TupleType(long line, TypeList* tupleTypes) : TupleType(tupleTypes) {
    setLine(line);
  }
  template <class InputIt>
  TupleType(long line, InputIt begin, InputIt end, bool isRef, bool isNilable)
      : TupleType(begin, end, isRef, isNilable) {
    setLine(line);
  }
  template <class InputIt>
  TupleType(InputIt begin, InputIt end, bool isRef, bool isNilable)
      : Type(isRef, isNilable), tupleTypes(new TypeList(begin, end)) {}
  TupleType(TypeList* tupleTypes, bool isRef, bool isNilable)
      : Type(isRef, isNilable), tupleTypes(tupleTypes) {}
  virtual ~TupleType() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;
  TypeList* elementTypes() { return tupleTypes; }
};
} // namespace node
} // namespace ast
#endif
