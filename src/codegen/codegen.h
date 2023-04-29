#ifndef SAPPHIRE_CODEGEN_CODEGEN_H_
#define SAPPHIRE_CODEGEN_CODEGEN_H_

#include "ast/ast.h"
#include "passes/ast-pass.h"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <string>

namespace codegen {

class LLVMCodegen : public pass::ASTPass {

  std::unique_ptr<llvm::LLVMContext> Context;
  std::unique_ptr<llvm::IRBuilder<>> Builder;
  std::unique_ptr<llvm::Module> Module;

public:
  using SymbolMap = std::
      unordered_map<ast::symbol::Symbol*, std::pair<llvm::Type*, llvm::Value*>>;

public:
  LLVMCodegen(ast::node::ASTNode* ast)
      : ASTPass(ast), Context(std::make_unique<llvm::LLVMContext>()),
        Builder(std::make_unique<llvm::IRBuilder<>>(*Context)),
        Module(std::make_unique<llvm::Module>("test", *Context)) {}

  std::string getIR() {
    std::string str;
    llvm::raw_string_ostream strm(str);
    Module->print(strm, nullptr);
    return str;
  }

private:
  void buildObjectFile(const std::string& filename);

public:
  void buildExecutable(const std::string& filename);

  bool run() override;
};
} // namespace codegen

#endif
