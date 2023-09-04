#ifndef SAPPHIRE_AST_OPERATORS_BUILTINS_H_
#define SAPPHIRE_AST_OPERATORS_BUILTINS_H_

#include "ast/ast.h"
#include "common/compiler-specific.h"
#include "ast/node/OperatorType.h

#include <cassert>
#include <functional>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <string>

namespace codegen {

namespace builtin {

class BuiltinDef;

using CodegenFunction = std::function<llvm::Function*(
    BuiltinDef* Builtin,
    llvm::LLVMContext* Context,
    llvm::IRBuilder<>* Builder,
    llvm::Module* Module)>;

#define BUILTIN_FORALL +1
constexpr std::size_t numBuiltins() {
  return 0
#include "builtin-list.inc"
      ;
}

extern BuiltinDef* builtins[numBuiltins()];

class BuiltinDef {

private:
  CodegenFunction codegenFunc_;

  bool isOperator_;
  std::string name_;
  // todo support builtin operators, might be better as a primitve call?
  // then i can have the builtin define a function like concat that
  // string+string calls as a primitve and a primitve like int+int can have a
  // default code gen, or use a user defined
  ast::node::OperatorType operatorType_;

  ast::node::CallableType* type_;

public:
  std::string name() {
    assert(!isOperator);
    return name_;
  }
  ast::node::OperatorType operatorType() {
    assert(isOperator);
    return operatorType_;
  };

  ast::node::CallableType* type() { return type_; }

public:
  BuiltinDef(
      CodegenFunction codegenFunc,
      const std::string& name,
      ast::node::Type* returnType,
      const ast::node::ASTList& parameterTypes)
      : codegenFunc_(codegenFunc), isOperator_(false), name_(name),
        operatorType_(),
        type_(ast::node::Type::getCallableType(returnType, parameterTypes)) {}

  BuiltinDef(
      CodegenFunction codegenFunc,
      ast::node::OperatorType operatorType,
      ast::node::Type* returnType,
      const ast::node::ASTList& parameterTypes)
      : codegenFunc_(codegenFunc), isOperator_(true), name_(),
        operatorType_(operatorType),
        type_(ast::node::Type::getCallableType(returnType, parameterTypes)) {}

  static BuiltinDef* createFunction(
      CodegenFunction codegenFunc,
      const std::string& name,
      ast::node::Type* returnType,
      const ast::node::ASTList& parameterTypes);
  static BuiltinDef*
  createFunction(CodegenFunction codegenFunc, const std::string& name);
  static BuiltinDef* createFunction(
      CodegenFunction codegenFunc,
      const std::string& name,
      ast::node::Type* returnType);

  DIAGNOSTIC_PUSH
  // if there is only one template arg, this complains about an unused value
  DIAGNOSTIC_IGNORE("-Wunused-value")
  template <typename... Ts>
  static BuiltinDef* createFunction(
      CodegenFunction codegenFunc,
      const std::string& name,
      ast::node::Type* returnType,
      Ts... parameterTypes) {
    static_assert(
        std::conjunction<std::bool_constant<(
            (std::is_pointer_v<Ts> &&
             std::is_base_of_v<Type, std::remove_pointer_t<Ts>>),
            ...)>>::value,
        "");
    ast::node::ASTList parameterTypesList;
    auto nOperands = 0;
    (([&] { parameterTypesList->push_back(parameterTypes); }()), ...);
    return Builtin::createFunction(
        codegenFunc,
        name,
        returnType,
        parameterTypesList);
  }
  DIAGNOSTIC_POP

  static BuiltinDef* createOperator(
      CodegenFunction codegenFunc,
      ast::node::OperatorType operatorType,
      ast::node::Type* returnType,
      const ast::node::ASTList& parameterTypes);

  DIAGNOSTIC_PUSH
  // if there is only one template arg, this complains about an unused value
  DIAGNOSTIC_IGNORE("-Wunused-value")
  template <typename... Ts>
  static BuiltinDef* createOperator(
      CodegenFunction codegenFunc,
      ast::node::OperatorType operatorType,
      ast::node::Type* returnType,
      Ts... parameterTypes) {
    static_assert(
        std::conjunction<std::bool_constant<(
            (std::is_pointer_v<Ts> &&
             std::is_base_of_v<Type, std::remove_pointer_t<Ts>>),
            ...)>>::value,
        "");
    ast::node::ASTList parameterTypesList;
    auto nOperands = 0;
    (([&] { parameterTypesList->push_back(parameterTypes); }()), ...);
    return Builtin::createOperator(
        codegenFunc,
        operatorType,
        returnType,
        parameterTypesList);
  }
  DIAGNOSTIC_POP

  virtual ~BuiltinDef() = default;
};

} // namespace builtin
} // namespace codegen
#endif
