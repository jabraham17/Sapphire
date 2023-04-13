
#include "codegen.h"

#include "ast/visitors/ast-visitor.h"

#include <iostream>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <unordered_map>

namespace codegen {

// ptr
static llvm::PointerType* PointerType(llvm::LLVMContext* Context) {
  return llvm::PointerType::getUnqual(*Context);
}

// {ptr, i64}
static llvm::StructType* StringType(llvm::LLVMContext* Context) {
  auto tt = llvm::StructType::getTypeByName(*Context, "spp_str");
  // if cannot find, create it
  if(tt == nullptr)
    tt = llvm::StructType::create(
        *Context,
        {PointerType(Context), llvm::Type::getInt64Ty(*Context)},
        "spp_str");
  return tt;
}

// {ptr, i64} = {elmType*, i64}
static llvm::StructType*
ArrayType(llvm::LLVMContext* Context, llvm::Type* elmType) {
  auto tt = llvm::StructType::getTypeByName(*Context, "spp_array");
  // if cannot find, create it
  if(tt == nullptr)
    tt = llvm::StructType::get(
        *Context,
        {PointerType(Context), llvm::Type::getInt64Ty(*Context)},
        "spp_array");
  return tt;
}

static llvm::Value* StackLocal(
    llvm::LLVMContext* Context,
    llvm::IRBuilder<>* Builder,
    llvm::Type* Type) {
  // save current BB
  llvm::BasicBlock* BB = Builder->GetInsertBlock();
  llvm::Function* F = BB->getParent();
  // we need the entry because all of our local vars must be alloca on the
  // entry for mem2reg to work properly
  auto& EntryBB = F->getEntryBlock();
  Builder->SetInsertPoint(&EntryBB);
  auto local = Builder->CreateAlloca(Type);
  // reset to BB
  Builder->SetInsertPoint(BB);
  return local;
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
      case ast::PrimitiveTypeEnum::STRING: set(StringType(Context)); break;
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

    // get element types LLVM type
    arg->elementType()->accept(this);
    auto elementType = this->returnValueAndClear();

    set(ArrayType(Context, elementType));
  }
};

static llvm::Type* getLLVMType(llvm::LLVMContext* Context, ast::Type* t) {
  TypeBuilder ltc(Context);
  t->accept(&ltc);
  return ltc.returnValue();
}

ast::Symbol* getSymbol(ast::ASTNode* ast) {
  if(auto a = ast::toUseExpressionNode(ast); a != nullptr) return a->symbol();
  return nullptr;
}

class ExpressionBuilder final : public ast::visitor::VisitorWithArgsAndReturn<
                                    ExpressionBuilder,
                                    ast::visitor::VisitAll,
                                    llvm::Value*,
                                    llvm::LLVMContext*,
                                    llvm::IRBuilder<>*,
                                    llvm::Module*,
                                    LLVMCodegen::SymbolMap*> {
public:
  using VisitorWithArgsAndReturn::VisitorWithArgsAndReturn;

protected:
  virtual void visitImpl(ast::StringExpression* arg) override {
    auto Context = get<0>();
    auto Builder = get<1>();

    auto literal = arg->escapedValue();
    auto stringType = StringType(Context);

    // allocate enough stack space for the string
    // TODO: should probably use an allocator here not stack memory
    auto stringStructPtr = StackLocal(Context, Builder, stringType);

    // store size into string
    auto sizeGEP = Builder->CreateStructGEP(stringType, stringStructPtr, 1);
    auto size = llvm::ConstantInt::get(
        llvm::Type::getInt64Ty(*Context),
        literal.size());
    Builder->CreateStore(size, sizeGEP);

    // alloc the literal string as a global and store in struct
    auto strPtr = Builder->CreateGlobalStringPtr(literal);
    auto strGEP = Builder->CreateStructGEP(stringType, stringStructPtr, 0);
    Builder->CreateStore(strPtr, strGEP);

    auto stringStruct = Builder->CreateLoad(stringType, stringStructPtr);

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

    auto exp = arg->expression();
    exp->accept(this);
    auto val = this->returnValueAndClear();

    auto ret = Builder->CreateRet(val);

    set(ret);
  }

  virtual void visitImpl(ast::ForStatement* arg) override {
    auto Context = get<0>();
    auto Builder = get<1>();
    auto Module = get<2>();
    auto variables = get<3>();

    llvm::BasicBlock* BB = Builder->GetInsertBlock();
    llvm::Function* F = BB->getParent();

    // Build 4 basic blocks
    // bb0: loop init
    // bb1: loop condition check and update var
    // bb2: loop body, this is our ScopeBuilder
    // bb3: loop cleanup, ie jump back to the top

    // init a loop counter and fully resolve the use-expr (the 'in' part)
    llvm::BasicBlock* BB0 = llvm::BasicBlock::Create(*Context, "", F);
    // insert explicit fallthrough from BB to BB0
    Builder->CreateBr(BB0);

    auto loopCount =
        StackLocal(Context, Builder, llvm::Type::getInt64Ty(*Context));

    // now in BB0, init index to 0
    Builder->SetInsertPoint(BB0);
    Builder->CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt64Ty(*Context), 0),
        loopCount);
    // here we should call the def expression
    arg->variable()->accept(this);
    auto defVar = this->returnValueAndClear();

    llvm::BasicBlock* BB1 = llvm::BasicBlock::Create(*Context, "", F);
    // insert explicit fallthrough from BB to BB0
    Builder->CreateBr(BB1);
    // now
  }

  virtual void visitImpl(ast::UseExpression* arg) override {
    auto Context = get<0>();
    auto Builder = get<1>();
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
      auto [llvmType, v_ptr] = variables->at(astSym);

      if(arg->isLHSOfAssign()) {
        // lhs, return a ptr
        set(v_ptr);
      } else {
        auto v = Builder->CreateLoad(llvmType, v_ptr);
        set(v);
      }
    }
  }

  virtual void visitImpl(ast::DefExpression* arg) override {
    auto Context = get<0>();
    auto Builder = get<1>();
    auto Module = get<2>();
    auto& variables = get<3>();

    auto astSym = arg->symbol();
    auto llvmType = getLLVMType(Context, astSym->type());
    auto valPtr = StackLocal(Context, Builder, llvmType);

    variables->insert_or_assign(astSym, std::make_pair(llvmType, valPtr));

    // TODO handle initial expr
  }

  std::string getStr(llvm::Value* v) {
    std::string str;
    llvm::raw_string_ostream strm(str);
    v->print(strm);
    return str;
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
      case ast::OperatorType::ASSIGNMENT: {
        auto astLHS = arg->operands()->get(0);
        auto astRHS = arg->operands()->get(1);

        astLHS->accept(this);
        // lhs is a ptr
        auto lhs = this->returnValueAndClear();
        astRHS->accept(this);
        auto rhs = this->returnValueAndClear();
        Builder->CreateStore(rhs, lhs);
        break;
      }
      case ast::OperatorType::SUBSCRIPT: {
        // TODO this will not work with tuples
        // TODO: a better solution may be to add different subscript ops for
        // array and tuples
        // TODO: but this probably has to be done during type resolution
        // TODO: probably want to do array bounds checking here

        auto astArray = arg->operands()->get(0);
        auto astIndex = arg->operands()->get(1);

        astArray->accept(this);
        auto array = this->returnValueAndClear();
        astIndex->accept(this);
        auto index = this->returnValueAndClear();

        ast::Type* astArrayType = ast::toExpressionNode(astArray)->type();
        assert(ast::toArrayTypeNode(astArrayType) != nullptr);
        ast::Type* astElmType =
            ast::toArrayTypeNode(astArrayType)->elementType();

        auto arrayType = getLLVMType(Context, astArrayType);
        auto elmType = getLLVMType(Context, astElmType);

        // store the array to the stack
        auto arrayPtr = StackLocal(Context, Builder, array->getType());
        Builder->CreateStore(array, arrayPtr);

        // extract raw array, which is just a pointer
        auto rawArrayGEP = Builder->CreateStructGEP(arrayType, arrayPtr, 0);
        auto rawArray = Builder->CreateLoad(PointerType(Context), rawArrayGEP);

        // get elm from rawArray
        auto elmGEP = Builder->CreateGEP(elmType, rawArray, index);
        auto elm = Builder->CreateLoad(elmType, elmGEP);
        set(elm);

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
                               LLVMCodegen::SymbolMap*> {
public:
  using VisitorWithArgs::VisitorWithArgs;

protected:
  virtual void visitImpl(ast::Scope* arg) override {
    auto Context = get<0>();
    auto Builder = get<1>();
    auto Module = get<2>();
    auto variables = get<3>();
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
        auto llvmType = getLLVMType(Context, tt);
        paramTypes.push_back(llvmType);

        paramNames.push_back(param->symbol()->name());
      } else {
        std::cerr << "error in codegen: type mismatch on param\n";
        exit(1);
      }
    }

    auto retType = getLLVMType(Context, arg->returnType());

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
    Builder->SetInsertPoint(BB);
    // build a map for function arguments
    // the scope will build its own local args to this
    LLVMCodegen::SymbolMap variables;
    auto params = arg->functionPrototype()->parameters();
    assert((F->arg_size()) == params->size());
    for(size_t i = 0; i < params->size(); i++) {
      if(auto param = ast::toParameterNode((*params)[i]); param != nullptr) {
        auto Arg = F->getArg(i);
        auto Sym = param->symbol();
        auto llvmType = Arg->getType();
        auto local = StackLocal(Context, Builder, llvmType);
        Builder->CreateStore(Arg, local);

        variables[Sym] = {llvmType, local};
      } else {
        std::cerr << "error in codegen: type mismatch on param\n";
        exit(1);
      }
    }

    ScopeBuilder sb(Context, Builder, Module, &variables);
    arg->body()->accept(&sb);

    if(F->getReturnType()->isVoidTy()) {
      Builder->CreateRetVoid();
    }

    // verify the body
    if(llvm::verifyFunction(*F, &llvm::errs())) {
      std::cerr << "\nERROR in function\n";
    }
  }
};

std::string LLVMCodegen::doCodegen(ast::ASTNode* ast) {

  // build all functions
  FunctionBuilder fb(Context.get(), Builder.get(), Module.get());
  ast->accept(&fb);

  return getIR();
}
} // namespace codegen
