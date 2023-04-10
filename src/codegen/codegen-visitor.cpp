#include "codegen-visitor.h"

#include <iostream>
#include <llvm/IR/Verifier.h>
#include <string>
#include <vector>

namespace codegen {

llvm::Type* CodegenVisitor::getLLVMType(ast::Type* t) {
  if(auto pt = ast::Type::toPrimitiveType(t); pt != nullptr) {
    switch(pt->primitiveType()) {
      case ast::PrimitiveTypeEnum::INT:
      case ast::PrimitiveTypeEnum::UINT:
        return llvm::Type::getInt64Ty(*Context);
      case ast::PrimitiveTypeEnum::REAL:
        return llvm::Type::getDoubleTy(*Context);
      case ast::PrimitiveTypeEnum::STRING:
        // a string is a pointer to the underlying buffer and a length
        return llvm::StructType::get(
            *Context,
            {llvm::Type::getInt8PtrTy(*Context),
             llvm::Type::getInt64Ty(*Context)});
      case ast::PrimitiveTypeEnum::BOOL: return llvm::Type::getInt8Ty(*Context);
      case ast::PrimitiveTypeEnum::BYTE: return llvm::Type::getInt8Ty(*Context);
      case ast::PrimitiveTypeEnum::NIL: return llvm::Type::getVoidTy(*Context);
      case ast::PrimitiveTypeEnum::UNKNOWN:
        std::cerr << "unknown type in conversion to LLVM\n";
        exit(1);
    }
  } else if(auto at = ast::Type::toArrayType(t); at != nullptr) {
    return llvm::StructType::get(
        *Context,
        {llvm::Type::getInt8PtrTy(*Context), llvm::Type::getInt64Ty(*Context)});
  } else {
    std::cerr << "type not yet handled " << ast::Type::getTypeString(t) << "\n";
    exit(1);
  }
  return nullptr;
}

#define VISIT(name) void CodegenVisitor::visit##name##Impl(ast::name* arg)
VISIT(FunctionPrototype) {
  // function type
  std::vector<llvm::Type*> paramTypes;
  std::vector<std::string> paramNames;
  for(auto elm : *arg->parameters()) {
    if(auto param = ast::toParameterNode(elm); param != nullptr) {
      auto tt = param->symbol()->type();
      paramTypes.push_back(getLLVMType(tt));
      paramNames.push_back(param->symbol()->name());
    } else {
      std::cerr << "error in codegen: type mismatch on param\n";
      exit(1);
    }
  }
  llvm::Type* retType = getLLVMType(arg->returnType());

  llvm::FunctionType* funcType =
      llvm::FunctionType::get(retType, paramTypes, false);

  llvm::Function* F = llvm::Function::Create(
      funcType,
      llvm::Function::ExternalLinkage,
      arg->name(),
      Module.get());

  // set arg names to the names for the params
  size_t i = 0;
  for(auto& a : F->args()) {
    a.setName(paramNames[i]);
    i++;
  }
}
VISIT(FunctionDefinition) {
  // codegen function def
  arg->functionPrototype()->accept(this);
  // get functions
  llvm::Function* F = Module->getFunction(arg->functionPrototype()->name());

  // main function body starts here
  llvm::BasicBlock* BB = llvm::BasicBlock::Create(*Context, "entry", F);
  Builder->SetInsertPoint(BB);

  // codegen the body
  arg->body()->accept(this);

  // verify the body
  llvm::verifyFunction(*F);
}

VISIT(Scope) {}

} // namespace codegen
