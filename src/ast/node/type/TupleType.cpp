// #include "TupleType.h"
// namespace ast {
// namespace node {

// ASTNode* TupleType::clone() {
//   auto xTupleTypes = toNodeType<std::remove_pointer_t<decltype(tupleTypes)>>(
//       tupleTypes->clone());
//   auto x = new TupleType(xTupleTypes, this->isRef(), this->isNilable());
//   return x;
// }

// TupleType::TupleType(TypeList* tupleTypes, bool isRef, bool isNilable)
//     : Type(isRef, isNilable), tupleTypes(tupleTypes) {
//   tupleTypes->parent() = this;
// }
// TypeList* TupleType::elementTypes() { return this->tupleTypes; }
// } // namespace node
// } // namespace ast
