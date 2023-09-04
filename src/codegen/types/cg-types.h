
#include "ast/ast.h"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <string>

namespace codegen {
namespace types {

// ptr
llvm::PointerType* PointerType(llvm::LLVMContext* Context);

// {ptr, i64}
llvm::StructType* StringType(llvm::LLVMContext* Context);

// {ptr, i64} = {elmType*, i64}
llvm::StructType* ArrayType(
    llvm::LLVMContext* Context,
    [[maybe_unused]] llvm::Type* elmType,
    const std::string& elmTypeName = "");

} // namespace types

llvm::Type* getLLVMType(llvm::LLVMContext* Context, ast::node::Type* t);
} // namespace codegen
