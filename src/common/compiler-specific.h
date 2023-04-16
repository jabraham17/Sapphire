#ifndef SAPPHIRE_COMMON_COMPILER_SPECIFIC_H_
#define SAPPHIRE_COMMON_COMPILER_SPECIFIC_H_

#define DO_PRAGMA(x) _Pragma(#x)

#ifdef __clang__
  #define DIAGNOSTIC_PUSH DO_PRAGMA(clang diagnostic push)
  #define DIAGNOSTIC_IGNORE(x) DO_PRAGMA(clang diagnostic ignored x)
  #define DIAGNOSTIC_POP DO_PRAGMA(clang diagnostic pop)
#else
  #ifdef __gcc__
    #define DIAGNOSTIC_PUSH DO_PRAGMA(GCC diagnostic push)
    #define DIAGNOSTIC_IGNORE(x) DO_PRAGMA(GCC diagnostic ignored x)
    #define DIAGNOSTIC_POP DO_PRAGMA(GCC diagnostic pop)
  #else
    #define DIAGNOSTIC_PUSH
    #define DIAGNOSTIC_IGNORE
    #define DIAGNOSTIC_POP
  #endif
#endif

#endif
