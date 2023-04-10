#ifndef SAPPHIRE_CODEGEN_CODEGEN_VISITOR_H_
#define SAPPHIRE_CODEGEN_CODEGEN_VISITOR_H_

#include "ast/ast.h"
#include "ast/visitors/visit-all.h"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <unordered_map>

namespace codegen {
class CodegenVisitor : public ast::visitor::VisitAll {
private:
  std::unique_ptr<llvm::LLVMContext> Context;
  std::unique_ptr<llvm::IRBuilder<>> Builder;
  std::unique_ptr<llvm::Module> Module;
  std::unordered_map<std::string, llvm::Value*> NamedValues;

public:
  CodegenVisitor()
      : Context(std::make_unique<llvm::LLVMContext>()),
        Builder(std::make_unique<llvm::IRBuilder<>>(*Context)),
        Module(std::make_unique<llvm::Module>("test", *Context)),
        NamedValues() {}
  virtual ~CodegenVisitor() = default;

  std::string getIR() {
    std::string str;
    llvm::raw_string_ostream strm(str);
    Module->print(strm, nullptr);
    return str;
  }

private:
  llvm::Type* getLLVMType(ast::Type* t);

protected:
#define ast_node_def_include
#define ast_node_def_include_FunctionDefinition
#define ast_node_def_include_FunctionPrototype
#define ast_node_def_include_Scope
#define ast_node_def(type)                                                     \
  virtual void visit##type##Impl(ast::type* arg) override;
#include "ast/ast-node.inc"
};
} // namespace codegen

#endif
