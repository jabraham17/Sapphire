#ifndef SAPPHIRE_CODEGEN_CODEGEN_H_
#define SAPPHIRE_CODEGEN_CODEGEN_H_

#include "ast/ast.h"

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

class Codegen {
public:
  Codegen() = default;
  virtual ~Codegen() = default;
  virtual std::string doCodegen(ast::node::ASTNode* ast) = 0;
};

class LLVMCodegen : Codegen {

  std::unique_ptr<llvm::LLVMContext> Context;
  std::unique_ptr<llvm::IRBuilder<>> Builder;
  std::unique_ptr<llvm::Module> Module;

public:
  using SymbolMap = std::
      unordered_map<ast::symbol::Symbol*, std::pair<llvm::Type*, llvm::Value*>>;

public:
  LLVMCodegen()
      : Context(std::make_unique<llvm::LLVMContext>()),
        Builder(std::make_unique<llvm::IRBuilder<>>(*Context)),
        Module(std::make_unique<llvm::Module>("test", *Context)) {}

  std::string getIR() {
    std::string str;
    llvm::raw_string_ostream strm(str);
    Module->print(strm, nullptr);
    return str;
  }

  virtual std::string doCodegen(ast::node::ASTNode* ast) override;
};
} // namespace codegen

#endif
