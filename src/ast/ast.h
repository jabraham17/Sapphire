#ifndef SAPPHIRE_AST_AST_H_
#define SAPPHIRE_AST_AST_H_

// only include the root
#include "ast/node/ASTNode.h"

#include <type_traits>
#include <typeinfo>

#define CONST_MEMBER_FUNC(retType, name) retType name();

// #define CONST_MEMBER_FUNC(retType, name)                                       \
//   retType name() const;                                                        \
//   retType name() {                                                             \
//     using ConstClassType = std::add_pointer_t<                                 \
//         std::add_const_t<std::remove_pointer_t<decltype(this)>>>;              \
//     return const_cast<ConstClassType>(this)->name();                           \
//   }

// #define CONST_MEMBER_FUNC_NO_RET(name)                                         \
//   void name() const;                                                           \
//   void name() {                                                                \
//     using ConstClassType = std::add_pointer_t<                                 \
//         std::add_const_t<std::remove_pointer_t<decltype(this)>>>;              \
//     const_cast<ConstClassType>(this)->name();                                  \
//   }

namespace ast {

namespace visitor {
class ASTVisitor;
}

namespace symbol {
class Symbol;
class FunctionSymbol;
} // namespace symbol

namespace node {
#define ast_node_def(type) class type;
#include "ast-node.inc"
} // namespace node

// defines the conversions
template <class T> T* toNodeType(node::ASTNode* a) {
  return dynamic_cast<T*>(a);
}
template <class T> bool isNodeType(node::ASTNode* a) {
  return toNodeType<T>(a) != nullptr;
}

} // namespace ast

#endif
