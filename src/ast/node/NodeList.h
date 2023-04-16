#ifndef SAPPHIRE_AST_NODE_NODELIST_H_
#define SAPPHIRE_AST_NODE_NODELIST_H_

#include "ast/ast.h"

#include <deque>
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
  using const_iter_type = decltype(elms)::const_iterator;

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

  NodeType* get(std::size_t idx) const;
  std::size_t size() const;
  const_iter_type begin() const;
  const_iter_type end() const;
  NodeType* operator[](std::size_t idx) const;

  NodeType* get(std::size_t idx) {
    return const_cast<const NodeList*>(this)->get(idx);
  }
  std::size_t size() { return const_cast<const NodeList*>(this)->size(); }
  iter_type begin();
  iter_type end();
  NodeType*& operator[](std::size_t idx);
};
// } // namespace detail

} // namespace node
} // namespace ast
#endif
