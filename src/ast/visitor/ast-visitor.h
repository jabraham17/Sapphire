#ifndef SAPPHIRE_AST_VISITOR_AST_VISITOR_H_
#define SAPPHIRE_AST_VISITOR_AST_VISITOR_H_

#include "ast/ast.h"

#include <tuple>
#include <type_traits>

namespace ast {
namespace visitor {

class ASTVisitor {
public:
  ASTVisitor() = default;
  virtual ~ASTVisitor() = default;
#define ast_node_def(type)                                                     \
protected:                                                                     \
  virtual void visitImpl(node::type*) {}                                       \
                                                                               \
public:                                                                        \
  void visit(node::type* a);
#include "ast/ast-node.inc"
};

class VisitAll : public ASTVisitor {

public:
  VisitAll() = default;
  virtual ~VisitAll() = default;

protected:
#define ast_node_def(type) virtual void visitImpl(node::type*) override;
#include "ast/ast-node.inc"
};

template <class Visitor, class ParentVisitor, typename... ParameterTypes>
class VisitorWithArgs : public ParentVisitor {
protected:
  std::tuple<ParameterTypes...> parameterValues_;
  template <std::size_t idx> auto& get() {
    return std::get<idx>(parameterValues_);
  }

public:
  VisitorWithArgs(std::tuple<ParameterTypes...> params)
      : parameterValues_(params) {}
  template <typename... Params>
  VisitorWithArgs(Params... params) : parameterValues_(params...) {}
  virtual ~VisitorWithArgs() = default;
};

template <
    class Visitor,
    class ParentVisitor,
    typename ReturnType,
    typename... ParameterTypes>
class VisitorWithArgsAndReturn
    : public VisitorWithArgs<Visitor, ParentVisitor, ParameterTypes...> {
public:
  using VisitorWithArgs<Visitor, ParentVisitor, ParameterTypes...>::
      VisitorWithArgs;
  virtual ~VisitorWithArgsAndReturn() = default;
  ReturnType returnValue() { return returnValue_; }
  ReturnType returnValueAndClear() {
    auto v = std::move(returnValue_);
    returnValue_ = {}; // reset to default value
    return v;
  }

protected:
  ReturnType returnValue_;
  void set(ReturnType returnValue) { returnValue_ = returnValue; }
};

} // namespace visitor
} // namespace ast

#endif
