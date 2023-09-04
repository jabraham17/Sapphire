
#include "BuiltinDef.h"

#include "ast/node/type/nodes.h"
#include "codegen/types/cg-types.h"

namespace codegen {
namespace builtin {
namespace codegenfunc {
llvm::Function* RuntimeFunction(
    BuiltinDef* Builtin,
    llvm::LLVMContext* Context,
    llvm::IRBuilder<>* Builder,
    llvm::Module* Module) {
  // Builder->
  std::vector<llvm::Type*> paramTypes;
  for(auto t : Builtin->type()->parameterTypes()) {
    auto llvmType = types::getLLVMType(Context, t);
    paramTypes.push_back(llvmType);
  }

  auto retType = getLLVMType(Context, Builtin->type()->returnType());

  llvm::FunctionType* funcType =
      llvm::FunctionType::get(retType, paramTypes, false);

  std::string name;
  name =

      llvm::Function* F = llvm::Function::Create(
          funcType,
          llvm::Function::ExternalLinkage,
          arg->name(),
          Module);
}

} // namespace codegenfunc
} // namespace builtin
} // namespace codegen
