

#ifdef BUILTIN_FORALL
  #define BUILTIN_RUNTIME(name, returnType, ...) BUILTIN_FORALL
  #define BUILTIN_LLVM(codegen, name, returnType, ...) BUILTIN_FORALL
#endif

#ifndef BUILTIN_RUNTIME
  #define BUILTIN_RUNTIME(name, returnType, ...)
#endif
#ifndef BUILTIN_LLVM
  #define BUILTIN_LLVM(codegen, name, returnType, ...)
#endif

#define VOID ast::node::Type::getNilType()
#define ANY ast::node::Type::getAnyType()
#define PRIM(prim)                                                             \
  ast::node::type::getPrimitiveType(ast::node::PrimitiveTypeEnum::prim)
#define ARRAY(elmType) ast::node::type::getArrayType(elmType)
#define BUILTIN_RUNTIME_VOID(name, ...) BUILTIN_RUNTIME(name, )

BUILTIN_RUNTIME("print", VOID, PRIM(STRING))
BUILTIN_LLVM(println, "println", VOID, PRIM(STRING))

#undef VOID
#undef ANY
#undef PRIM
#undef ARRAY

#undef BUILTIN_FORALL
#undef BUILTIN_RUNTIME
#undef BUILTIN_LLVM
