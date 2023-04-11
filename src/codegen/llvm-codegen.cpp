
#include "codegen.h"

#include "ast/visitors/ast-visitor.h"

#include <iostream>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <unordered_map>

namespace codegen {

static llvm::Type* StringType(llvm::LLVMContext* Context) {
  return llvm::StructType::get(
      *Context,
      {llvm::Type::getInt8PtrTy(*Context), llvm::Type::getInt64Ty(*Context)});
}

class TypeBuilder final : public ast::visitor::VisitorWithArgsAndReturn<
                              TypeBuilder,
                              ast::visitor::ASTVisitor,
                              llvm::Type*,
                              llvm::LLVMContext*> {
public:
  using VisitorWithArgsAndReturn::VisitorWithArgsAndReturn;

protected:
  virtual void visitImpl(ast::Type* arg) override {
    std::cerr << "type not yet handled " << ast::Type::getTypeString(arg)
              << "\n";
    exit(1);
    set(nullptr);
  }
  virtual void visitImpl(ast::PrimitiveType* arg) override {
    auto Context = get<0>();
    switch(arg->primitiveType()) {
      case ast::PrimitiveTypeEnum::INT:
      case ast::PrimitiveTypeEnum::UINT:
        set(llvm::Type::getInt64Ty(*Context));
        break;
      case ast::PrimitiveTypeEnum::REAL:
        set(llvm::Type::getDoubleTy(*Context));
        break;
      case ast::PrimitiveTypeEnum::STRING:
        // a string is a pointer to the underlying buffer and a length
        set(StringType(Context));
        break;
      case ast::PrimitiveTypeEnum::BOOL:
        set(llvm::Type::getInt8Ty(*Context));
        break;
      case ast::PrimitiveTypeEnum::BYTE:
        set(llvm::Type::getInt8Ty(*Context));
        break;
      case ast::PrimitiveTypeEnum::NIL:
        set(llvm::Type::getVoidTy(*Context));
        break;
      case ast::PrimitiveTypeEnum::UNKNOWN:
        std::cerr << "unknown type in conversion to LLVM\n";
        exit(1);
        set(nullptr);
    }
  }
  virtual void visitImpl(ast::ArrayType* arg) override {
    auto Context = get<0>();

    arg->elementType()->accept(this);
    auto elementType = this->returnValueAndClear();

    set(elementType->getPointerTo());
    // set(llvm::StructType::get(
    //     *Context,
    //     {llvm::Type::getInt8PtrTy(*Context),
    //      llvm::Type::getInt64Ty(*Context)}));
  }
};

// class ConstantBuilder final : public ast::visitor::VisitorWithArgsAndReturn<
//                                   ScopeBuilder,
//                                   ast::visitor::ASTVisitor,
//                                   llvm::Value*,
//                                   llvm::LLVMContext*,
//                                   llvm::IRBuilder<>*,
//                                   llvm::Module*> {
// public:
//   using VisitorWithArgsAndReturn::VisitorWithArgsAndReturn;

// protected:
//   // build a local string
//   virtual void visitImpl(ast::StringExpression* arg) override {
//     auto Context = get<0>();
//     auto Builder = get<1>();
//     auto Module = get<2>();

//     auto literal = arg->value();
//     auto stringType = StringType(Context);

//     // allocate enough stack space for the string
//     // TODO: should probably use an allocator here not stack memory
//     auto stringStruct = Builder->CreateAlloca(stringType);

//     // store size into string
//     auto sizeGEP = Builder->CreateStructGEP(stringType, stringStruct, 1);
//     auto size = llvm::ConstantInt::get(sizeGEP->getType(), literal.size());
//     Builder->CreateStore(size, sizeGEP);

//     // alloc the literal string as a global and store in struct
//     auto strPtr = Builder->CreateGlobalStringPtr(literal);
//     auto strGEP = Builder->CreateStructGEP(stringType, stringStruct, 0);
//     Builder->CreateStore(strPtr, strGEP);

//     // return string struct
//     set(stringStruct);
//   }
// };

ast::Symbol* getSymbol(ast::ASTNode* ast) {
  if(auto a = ast::toUseExpressionNode(ast); a != nullptr) return a->symbol();
  return nullptr;
}

class ExpressionBuilder final
    : public ast::visitor::VisitorWithArgsAndReturn<
          ExpressionBuilder,
          ast::visitor::VisitAll,
          llvm::Value*,
          llvm::LLVMContext*,
          llvm::IRBuilder<>*,
          llvm::Module*,
          std::unordered_map<std::string, llvm::Value*>*> {
public:
  using VisitorWithArgsAndReturn::VisitorWithArgsAndReturn;

protected:
  // llvm::Value* buildConstant(ast::ASTNode* constantNode) {
  //   ConstantBuilder cp(get<0>(), get<1>(), get<2>());
  //   constantNode->accept(&cp);
  //   return cp.returnValue();
  // }
  // build a local string
  virtual void visitImpl(ast::StringExpression* arg) override {
    auto Context = get<0>();
    auto Builder = get<1>();

    auto literal = arg->escapedValue();
    auto stringType = StringType(Context);

    // allocate enough stack space for the string
    // TODO: should probably use an allocator here not stack memory
    auto stringStruct = Builder->CreateAlloca(stringType);

    // store size into string
    auto sizeGEP = Builder->CreateStructGEP(stringType, stringStruct, 1);
    auto size = llvm::ConstantInt::get(
        llvm::Type::getInt64Ty(*Context),
        literal.size());
    Builder->CreateStore(size, sizeGEP);

    // alloc the literal string as a global and store in struct
    auto strPtr = Builder->CreateGlobalStringPtr(literal);
    // auto strPtr = Builder->CreateGEP(llvm::Type::getInt8PtrTy(*Context), str,
    // llvm::ConstantInt::get(*Context, llvm::APInt(64, 0)));
    auto strGEP = Builder->CreateStructGEP(stringType, stringStruct, 0);
    Builder->CreateStore(strPtr, strGEP);

    // return string struct
    set(stringStruct);
  }
  virtual void visitImpl(ast::IntExpression* arg) override {
    auto Context = get<0>();
    auto Builder = get<1>();

    auto literal = arg->value();
    auto numConstant =
        llvm::ConstantInt::get(llvm::Type::getInt64Ty(*Context), literal);

    // return num
    set(numConstant);
  }

  virtual void visitImpl(ast::ReturnStatement* arg) override {
    auto Context = get<0>();
    auto Builder = get<1>();

    arg->expression()->accept(this);
    auto val = this->returnValueAndClear();
    auto ret = Builder->CreateRet(val);

    set(ret);
  }

  virtual void visitImpl(ast::UseExpression* arg) override {
    auto Module = get<2>();
    auto variables = get<3>();
    // TODO: if symbol a func, get a func. if symbol a var, get a var
    // right now just gets var

    auto astSym = arg->symbol();
    if(ast::Type::isCallableType(astSym->type())) {
      //  TODO: this doesnt yet handle mangling I dont think
      // TODO: also this if will never call because no type resolution :)
      auto F = Module->getFunction(astSym->name());
      set(F);
    } else {
      auto v = variables->at(astSym->name());
      set(v);
    }
  }

  virtual void visitImpl(ast::CallExpression* arg) override {
    auto Context = get<0>();
    auto Builder = get<1>();
    auto Module = get<2>();

    switch(arg->op()->opType()) {
      case ast::OperatorType::FUNCTION: {

        std::vector<llvm::Value*> functionArgs;
        // first operand is function name
        // TODO: add proper resolution to a function with a visitor based on
        // type
        auto astFuncToCall = ast::toUseExpressionNode(arg->operands()->get(0));
        // auto astFuncToCall = arg->operands()->get(0);
        // astFuncToCall->accept(this);
        // auto funcToCall = this->returnValueAndClear();
        auto funcToCall = Module->getFunction(astFuncToCall->symbol()->name());
        // funcToCall->print(llvm::errs());

        for(auto it = arg->operands()->begin() + 1;
            it != arg->operands()->end();
            it++) {
          (*it)->accept(this);
          auto val = this->returnValueAndClear();
          functionArgs.push_back(val);
        }

        auto funcCall = Builder->CreateCall(funcToCall, functionArgs);
        set(funcCall);
        break;
      }
      case ast::OperatorType::SUBSCRIPT: {
        auto astArrayOrTuple = arg->operands()->get(0);
        auto astIndex = arg->operands()->get(1);

        astArrayOrTuple->accept(this);
        auto arrayOrTuple = this->returnValueAndClear();

        // if(auto astArray = ast::)
        // TypeBuilder tb(Context);
        // astArrayOrTuple->accept(&tb);
        // auto arrayOrTupleType = tb.returnValue();

        auto astSymbol = getSymbol(astArrayOrTuple);
        llvm::Type* elmType = nullptr;
        if(auto astArrayType = ast::Type::toArrayType(astSymbol->type());
           astArrayType != nullptr) {
          auto astElmType = astArrayType->elementType();
          TypeBuilder tb(Context);
          astElmType->accept(&tb);
          elmType = tb.returnValue();
        }

        astIndex->accept(this);
        auto index = this->returnValueAndClear();

        // TODO: probably want to do array bounds checking here

        elmType->print(llvm::errs());
        std::cerr << "\n";
        arrayOrTuple->print(llvm::errs());
        std::cerr << "\n";
        auto gep = Builder->CreateGEP(elmType, arrayOrTuple, index);
        // auto elm = Builder->CreateLoad()

        set(gep);

        break;
      }
      default: std::cerr << "not yet handled\n"; exit(1);
    }
  }
};

class ScopeBuilder final : public ast::visitor::VisitorWithArgs<
                               ScopeBuilder,
                               ast::visitor::ASTVisitor,
                               llvm::LLVMContext*,
                               llvm::IRBuilder<>*,
                               llvm::Module*,
                               llvm::BasicBlock*,
                               std::unordered_map<std::string, llvm::Value*>*> {
public:
  using VisitorWithArgs::VisitorWithArgs;

protected:
  virtual void visitImpl(ast::Scope* arg) override {
    auto Context = get<0>();
    auto Builder = get<1>();
    auto Module = get<2>();
    auto BB = get<3>();
    auto variables = get<4>();

    Builder->SetInsertPoint(BB);
    for(auto elm : *arg->statements()) {
      ExpressionBuilder ep(Context, Builder, Module, variables);
      elm->accept(&ep);
    }
  }
};

class FunctionPrototypeBuilder final
    : public ast::visitor::VisitorWithArgsAndReturn<
          FunctionPrototypeBuilder,
          ast::visitor::ASTVisitor,
          llvm::Function*,
          llvm::LLVMContext*,
          llvm::Module*> {
public:
  using VisitorWithArgsAndReturn::VisitorWithArgsAndReturn;

protected:
  virtual void visitImpl(ast::FunctionPrototype* arg) override {
    auto Context = get<0>();
    auto Module = get<1>();
    // function type
    std::vector<llvm::Type*> paramTypes;
    std::vector<std::string> paramNames;
    for(auto elm : *arg->parameters()) {
      if(auto param = ast::toParameterNode(elm); param != nullptr) {

        auto tt = param->symbol()->type();
        TypeBuilder ltc(Context);
        tt->accept(&ltc);
        auto llvmType = ltc.returnValue();
        // struct types as arguments must be passed as pointers
        if(llvmType->isStructTy()) llvmType = llvmType->getPointerTo();
        paramTypes.push_back(llvmType);

        paramNames.push_back(param->symbol()->name());
      } else {
        std::cerr << "error in codegen: type mismatch on param\n";
        exit(1);
      }
    }

    TypeBuilder ltc(Context);
    arg->returnType()->accept(&ltc);
    auto retType = ltc.returnValue();

    llvm::FunctionType* funcType =
        llvm::FunctionType::get(retType, paramTypes, false);

    llvm::Function* F = llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        arg->mangledNamed(),
        Module);

    // set arg names to the names for the params
    size_t i = 0;
    for(auto& a : F->args()) {
      a.setName(paramNames[i]);
      i++;
    }
    set(F);
  }
};

class FunctionBuilder final : public ast::visitor::VisitorWithArgs<
                                  FunctionBuilder,
                                  ast::visitor::VisitAll,
                                  llvm::LLVMContext*,
                                  llvm::IRBuilder<>*,
                                  llvm::Module*> {
public:
  using VisitorWithArgs::VisitorWithArgs;

protected:
  virtual void visitImpl(ast::ExternDefinition* arg) override {
    auto Context = get<0>();
    auto Module = get<2>();
    //  need to generate the prototype for externs
    FunctionPrototypeBuilder fpb(Context, Module);
    arg->functionPrototype()->accept(&fpb);
  }

  virtual void visitImpl(ast::FunctionDefinition* arg) override {
    auto Context = get<0>();
    auto Builder = get<1>();
    auto Module = get<2>();

    // get functions
    llvm::Function* F = Module->getFunction(arg->functionPrototype()->name());
    if(F == nullptr) {
      //  need to generate the prototype
      FunctionPrototypeBuilder fpb(Context, Module);
      arg->functionPrototype()->accept(&fpb);
      F = fpb.returnValue();
    }

    // main function body starts here
    llvm::BasicBlock* BB = llvm::BasicBlock::Create(*Context, "entry", F);

    // build a map for function arguments
    // the scope will build its own local args to this
    std::unordered_map<std::string, llvm::Value*> variables;
    for(auto& Arg : F->args())
      variables[std::string(Arg.getName())] = &Arg;

    ScopeBuilder sb(Context, Builder, Module, BB, &variables);
    arg->body()->accept(&sb);

    // verify the body
    llvm::verifyFunction(*F);
  }
};

std::string LLVMCodegen::doCodegen(ast::ASTNode* ast) {

  // build all functions
  FunctionBuilder fb(Context.get(), Builder.get(), Module.get());
  ast->accept(&fb);

  return getIR();
}
} // namespace codegen
