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
#define ast_node_def_exclude
#define ast_node_def_exclude_Parameter
#define ast_node_def_exclude_Statement
#define ast_node_def_exclude_Type
#define ast_node_def_exclude_PrimitiveType
#define ast_node_def_exclude_ClassType
#define ast_node_def_exclude_UseExpression
#define ast_node_def_exclude_IntExpression
#define ast_node_def_exclude_UIntExpression
#define ast_node_def_exclude_RealExpression
#define ast_node_def_exclude_StringExpression
#define ast_node_def_exclude_Nil
#define ast_node_def(type) virtual void visitImpl(node::type*) override;
#include "ast/ast-node.inc"

// these all do nothing, as they have no children to search through
#define ast_node_def_include
#define ast_node_def_include_Parameter
#define ast_node_def_include_Statement
#define ast_node_def_include_Type
#define ast_node_def_include_PrimitiveType
#define ast_node_def_include_ClassType
#define ast_node_def_include_UseExpression
#define ast_node_def_include_IntExpression
#define ast_node_def_include_UIntExpression
#define ast_node_def_include_RealExpression
#define ast_node_def_include_StringExpression
#define ast_node_def_include_Nil
#define ast_node_def(type) virtual void visitImpl(node::type*) override{};
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