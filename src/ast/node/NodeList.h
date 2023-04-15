#ifndef SAPPHIRE_AST_NODE_NODELIST_H_
#define SAPPHIRE_AST_NODE_NODELIST_H_
#include "ASTNode.h"

#include "ast/ast.h"

#include <type_traits>

namespace ast {
namespace node {
// namespace detail {
// template <typename Derived, typename NodeType>
class NodeList : public ASTNode {
private:
  using NodeType = ASTNode;
  std::deque<NodeType*> elms;
  using iter_type = decltype(elms)::iterator;

public:
  NodeList() : elms() {}
  // template <
  //     class InputIt,
  //     typename std::enable_if_t<std::is_same<
  //         typename std::iterator_traits<InputIt>::value_type,
  //         SomeType>>>
  // NodeList(InputIt begin, InputIt end) {}

  virtual ~NodeList() = default;
  virtual void accept(visitor::ASTVisitor* ast) override;

  void addFront(NodeType* elm);
  void addBack(NodeType* elm);

  NodeType* get(size_t idx) const;
  size_t size() const;
  iter_type begin() const;
  iter_type end() const;
  NodeType*& operator[](size_t idx) const;

  NodeType* get(size_t idx) {
    return const_cast<const NodeList*>(this)->get(idx);
  }
  size_t size() { return const_cast<const NodeList*>(this)->size(); }
  iter_type begin() { return const_cast<const NodeList*>(this)->begin(); }
  iter_type end() { return const_cast<const NodeList*>(this)->end(); }
  NodeType*& operator[](size_t idx) {
    return (*const_cast<const NodeList*>(this))[idx];
  }
};
// } // namespace detail

} // namespace node
} // namespace ast
#endif
