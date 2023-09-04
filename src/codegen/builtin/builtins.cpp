#include "BuiltinDef.h"

namespace codegen {

namespace builtin {

#define BUILTIN_RUNTIME(name, returnType, ...)                                 \
  BuiltinDef::createFunction(RuntimeFunction, name, returnType, __VA_ARGS__),
#define BUILTIN_LLVM(codegen, name, returnType, ...)                           \
  BuiltinDef::createFunction(codegen, name, returnType, __VA_ARGS__),

BuiltinDef* builtins[numBuiltins()] = {
#include "builtin-list.inc"
};

} // namespace builtin
} // namespace codegen
