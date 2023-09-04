#include "cg-types.h"

#include "ast/node/type/nodes.h"
#include "ast/visitor/ast-visitor.h"

#include <iostream>
namespace codegen {
namespace types {

// ptr
llvm::PointerType* PointerType(llvm::LLVMContext* Context) {
  return llvm::PointerType::getUnqual(*Context);
}

// {ptr, i64}
llvm::StructType* StringType(llvm::LLVMContext* Context) {
  auto tt = llvm::StructType::getTypeByName(*Context, "spp_string");
  // if cannot find, create it
  if(tt == nullptr)
    tt = llvm::StructType::create(
        *Context,
        {PointerType(Context), llvm::Type::getInt64Ty(*Context)},
        "spp_string");
  return tt;
}

// {ptr, i64} = {elmType*, i64}
llvm::StructType* ArrayType(
    llvm::LLVMContext* Context,
    [[maybe_unused]] llvm::Type* elmType,
    const std::string& elmTypeName) {
  auto name = "spp_array_" + elmTypeName;
  auto tt = llvm::StructType::getTypeByName(*Context, name);
  // if cannot find, create it
  if(tt == nullptr)
    tt = llvm::StructType::create(
        *Context,
        {PointerType(Context), llvm::Type::getInt64Ty(*Context)},
        name);
  return tt;
}

class TypeBuilder final : public ast::visitor::VisitorWithArgsAndReturn<
                              TypeBuilder,
                              ast::visitor::ASTVisitor,
                              llvm::Type*,
                              llvm::LLVMContext*> {
public:
  using VisitorWithArgsAndReturn::VisitorWithArgsAndReturn;

protected:
  virtual void visitImpl(ast::node::Type* arg) override {
    std::cerr << "type not yet handled " << arg->toString() << "\n";
    exit(1);
    set(nullptr);
  }
  virtual void visitImpl(ast::node::PrimitiveType* arg) override {
    auto Context = get<0>();
    switch(arg->primitiveType()) {
      case ast::node::PrimitiveTypeEnum::INT:
      case ast::node::PrimitiveTypeEnum::UINT:
        set(llvm::Type::getInt64Ty(*Context));
        break;
      case ast::node::PrimitiveTypeEnum::REAL:
        set(llvm::Type::getDoubleTy(*Context));
        break;
      case ast::node::PrimitiveTypeEnum::STRING:
        set(StringType(Context));
        break;
      case ast::node::PrimitiveTypeEnum::BOOL:
        set(llvm::Type::getInt8Ty(*Context));
        break;
      case ast::node::PrimitiveTypeEnum::BYTE:
        set(llvm::Type::getInt8Ty(*Context));
        break;
      case ast::node::PrimitiveTypeEnum::NIL:
        set(llvm::Type::getVoidTy(*Context));
        break;
      case ast::node::PrimitiveTypeEnum::UNTYPED:
        std::cerr << "cannot convert UNTYPED type to LLVM\n";
        exit(1);
      case ast::node::PrimitiveTypeEnum::ANY:
        std::cerr << "cannot convert ANY type to LLVM\n";
        exit(1);
      case ast::node::PrimitiveTypeEnum::UNKNOWN:
        std::cerr << "cannot convert UNKNOWN type to LLVM\n";
        exit(1);
    }
  }
  virtual void visitImpl(ast::node::ArrayType* arg) override {
    auto Context = get<0>();

    // get element types LLVM type
    arg->elementType()->accept(this);
    auto elementType = this->returnValueAndClear();

    set(ArrayType(Context, elementType, arg->elementType()->toString()));
  }
};

llvm::Type* getLLVMType(llvm::LLVMContext* Context, ast::node::Type* t) {
  TypeBuilder ltc(Context);
  t->accept(&ltc);
  return ltc.returnValue();
}

} // namespace types
} // namespace codegen
