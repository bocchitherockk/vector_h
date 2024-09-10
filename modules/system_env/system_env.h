#ifndef SYSTEM_ENV_H
#define SYSTEM_ENV_H

// Define macros for compiler checks
#if defined(__clang__)  // Clang compiler
    #define COMPILER_IS_CLANG 1
    #define COMPILER_IS_GCC 0
    #define COMPILER_IS_TCC 0
    #define COMPILER_IS_MSVC 0
    #define COMPILER_IS_INTEL 0
    #define COMPILER_IS_UNKNOWN 0
#elif defined(__GNUC__)  // GCC compiler
    #define COMPILER_IS_CLANG 0
    #define COMPILER_IS_GCC 1
    #define COMPILER_IS_TCC 0
    #define COMPILER_IS_MSVC 0
    #define COMPILER_IS_INTEL 0
    #define COMPILER_IS_UNKNOWN 0
#elif defined(__TINYC__) // Tiny C compiler
    #define COMPILER_IS_CLANG 0
    #define COMPILER_IS_GCC 0
    #define COMPILER_IS_TCC 1
    #define COMPILER_IS_MSVC 0
    #define COMPILER_IS_INTEL 0
    #define COMPILER_IS_UNKNOWN 0
#elif defined(_MSC_VER)  // Microsoft Visual C++ compiler
    #define COMPILER_IS_CLANG 0
    #define COMPILER_IS_GCC 0
    #define COMPILER_IS_TCC 0
    #define COMPILER_IS_MSVC 1
    #define COMPILER_IS_INTEL 0
    #define COMPILER_IS_UNKNOWN 0
    #warning i have to test this on windows msvc compiler
#elif defined(__INTEL_COMPILER)  // Intel C++ compiler
    #define COMPILER_IS_CLANG 0
    #define COMPILER_IS_GCC 0
    #define COMPILER_IS_TCC 0
    #define COMPILER_IS_MSVC 0
    #define COMPILER_IS_INTEL 1
    #define COMPILER_IS_UNKNOWN 0
    #warning who the fuck uses an intel compiler
#else  // Unknown compiler
    #define COMPILER_IS_CLANG 0
    #define COMPILER_IS_GCC 0
    #define COMPILER_IS_TCC 0
    #define COMPILER_IS_MSVC 0
    #define COMPILER_IS_INTEL 0
    #define COMPILER_IS_UNKNOWN 1
    #warning you are using an unknown compiler, feel free to add its comfiguration
#endif
//    "need to check for these compilers": ["intel compiler", "msvc"]
/*{
    "statement expressions": ["clang", "gcc", "tcc"],
    "'typeof' instruction": ["clang", "gcc", "tcc"],
    "nested functions": ["gcc"],
    "__builtin_clz and its dirivatives": ["clang", "gcc"]
}*/

#define COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS (COMPILER_IS_CLANG || COMPILER_IS_GCC || COMPILER_IS_TCC)
#define COMPILER_SUPPORTS_TYPEOF (COMPILER_IS_CLANG || COMPILER_IS_GCC || COMPILER_IS_TCC)
#define COMPILER_SUPPORTS_NESTED_FUNCTIONS COMPILER_IS_GCC
#define COMPILER_SUPPORTS___BUILTIN_CLZ (COMPILER_IS_CLANG || COMPILER_IS_GCC) // and its derivatives

#endif // SYSTEM_ENV_H
