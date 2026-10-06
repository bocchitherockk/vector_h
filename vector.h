#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>


/////////////////////////////////// ASSERTF_H ///////////////////////////////////
#ifndef ASSERTF_H
#define ASSERTF_H

static inline void assertf_impl(const char *file, int line, const char *func, const char *fmt, ...) {
    va_list args;
    fprintf(stderr, "%s:%d: in %s(): ", file, line, func);
    if (*fmt != '\0') { // if not an empty string ""
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
    }
    exit(1);
}

#define assertf(cond, ...) do {                                     \
    if (!(cond)) {                                                  \
        assertf_impl(__FILE__, __LINE__, __func__, "" __VA_ARGS__); \
    }                                                               \
} while(0)

#endif // ASSERTF_H
/////////////////////////////////// ASSERTF_H ///////////////////////////////////


/////////////////////////////////// SYSTEM_ENV_H ///////////////////////////////////
/////////////////////////////////// COMPILER ///////////////////////////////////
#ifndef SYSTEM_ENV_H
#define SYSTEM_ENV_H

#define COMPILER_EMSCRIPTEN 0
#define COMPILER_CLANG 0
#define COMPILER_INTEL 0
#define COMPILER_TCC 0
#define COMPILER_MSVC 0
#define COMPILER_ARM 0
#define COMPILER_MINGW 0
#define COMPILER_GCC 0
#define COMPILER_UNKNOWN 0

#if defined(__EMSCRIPTEN__)
    #define COMPILER_NAME "emscripten"
    #undef COMPILER_EMSCRIPTEN
    #define COMPILER_EMSCRIPTEN 1
    #define COMPILER_VERSION_MAJOR 0
    #define COMPILER_VERSION_MINOR 0
    #define COMPILER_VERSION_PATCH 0
    // As of my knowledge, emscripten does not define version macros
#elif defined(__clang__)
    #define COMPILER_NAME "clang"
    #undef COMPILER_CLANG
    #define COMPILER_CLANG 1
    #define COMPILER_VERSION_MAJOR __clang_major__
    #define COMPILER_VERSION_MINOR __clang_minor__
    #define COMPILER_VERSION_PATCH __clang_patchlevel__

#elif defined(__INTEL_COMPILER) || defined(__ICL)
    // __ICC and __ECC are obsolete
    // __INTEL_COMPILER = VRP; V = Version; R = Revision; P = Patch
    #define COMPILER_NAME "intel"
    #undef COMPILER_INTEL
    #define COMPILER_INTEL 1
    #define COMPILER_VERSION_MAJOR (__INTEL_COMPILER / 100)
    #define COMPILER_VERSION_MINOR ((__INTEL_COMPILER / 10) % 10)
    #define COMPILER_VERSION_PATCH (__INTEL_COMPILER % 10)
    #pragma message("this compiler is not tested yet.")

#elif defined(__TINYC__)
    #define COMPILER_NAME "tcc"
    #undef COMPILER_TCC
    #define COMPILER_TCC 1
    #define COMPILER_VERSION_MAJOR 0 // TCC does not define version macros
    #define COMPILER_VERSION_MINOR 0
    #define COMPILER_VERSION_PATCH 0

#elif defined(_MSC_VER)
    #define COMPILER_NAME "msvc"
    #undef COMPILER_MSVC
    #define COMPILER_MSVC 1
    #define COMPILER_VERSION_MAJOR (_MSC_VER / 100)
    #define COMPILER_VERSION_MINOR (_MSC_VER % 100)
    // the patch starting from Visual C++ 6.0 (_MSC_VER = 1200) is represented on 4 digits.
    // the patch starting from Visual C++ 8.0 (_MSC_VER = 1400) is represented on 5 digits.
    #if _MSC_VER >= 1400
        #define COMPILER_VERSION_PATCH (_MSC_FULL_VER % 100000)
    #elif _MSC_VER >= 1200
        #define COMPILER_VERSION_PATCH (_MSC_FULL_VER % 10000)
    #else
        #define COMPILER_VERSION_PATCH 0
    #endif

#elif defined(__CC_ARM) && defined(__ARMCC_VERSION)
    // __ARMCC_VERSION = VRPBBB; V = Version; R = Revision; P = Patch; BBB = Build
    #define COMPILER_NAME "arm"
    #undef COMPILER_ARM
    #define COMPILER_ARM 1
    #define COMPILER_VERSION_MAJOR (__ARMCC_VERSION / 100000)
    #define COMPILER_VERSION_MINOR ((__ARMCC_VERSION / 10000) % 10)
    #define COMPILER_VERSION_PATCH ((__ARMCC_VERSION / 1000) % 10)
    #pragma message("this compiler is not tested yet.")

#elif defined(__MINGW64__)
    #define COMPILER_NAME "mingw64"
    #undef COMPILER_MINGW
    #define COMPILER_MINGW 1
    #define COMPILER_VERSION_MAJOR __MINGW64_VERSION_MAJOR
    #define COMPILER_VERSION_MINOR __MINGW64_VERSION_MINOR
    #define COMPILER_VERSION_PATCH 0 // MinGW does not define patch level
    #pragma message("this compiler is not tested yet.")
    // i don't know if this is even a thing

#elif defined(__MINGW32__)
    #define COMPILER_NAME "mingw32"
    #undef COMPILER_MINGW
    #define COMPILER_MINGW 1
    #define COMPILER_VERSION_MAJOR __MINGW32_MAJOR_VERSION
    #define COMPILER_VERSION_MINOR __MINGW32_MINOR_VERSION
    #define COMPILER_VERSION_PATCH 0 // MinGW does not define patch level

#elif defined(__GNUC__)
    // i put this at the end because i think even clang and intel define this macro
    #define COMPILER_NAME "gcc"
    #undef COMPILER_GCC
    #define COMPILER_GCC 1
    #define COMPILER_VERSION_MAJOR __GNUC__
    #define COMPILER_VERSION_MINOR __GNUC_MINOR__
    #if defined(__GNUC_PATCHLEVEL__)
        #define COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__
    #else
        #define COMPILER_VERSION_PATCH 0
    #endif

#else
    #define COMPILER_NAME "unknown"
    #undef COMPILER_UNKNOWN
    #define COMPILER_UNKNOWN 1
    #pragma message("Warning: Unknown compiler detected, please add its configuration.")
#endif


// Feature Detection Based on Previously Detected Compiler
// note: as i said others that have not been tested yet are not included
// note: i'll be adding more features as i need them
/* mingw is based on gcc */
/* emscripten is based on clang */
/* msvc was tested, and it supports none of those features */
#define COMPILER_SUPPORTS_TYPEOF                (COMPILER_GCC || COMPILER_CLANG || COMPILER_TCC || COMPILER_MINGW || COMPILER_EMSCRIPTEN)
#define COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS (COMPILER_GCC || COMPILER_CLANG || COMPILER_TCC || COMPILER_MINGW || COMPILER_EMSCRIPTEN)
#define COMPILER_SUPPORTS_NESTED_FUNCTIONS      (COMPILER_GCC || COMPILER_MINGW)
#define COMPILER_SUPPORTS_BUILTIN_CLZ           (COMPILER_GCC || COMPILER_CLANG || COMPILER_MINGW || COMPILER_EMSCRIPTEN)
/////////////////////////////////// COMPILER ///////////////////////////////////

/////////////////////////////////// LANGUAGE ///////////////////////////////////
#define LANGUAGE_C 0
#define LANGUAGE_CPP 0

#ifdef __cplusplus
    #define LANGUAGE_NAME "C++"
    #undef LANGUAGE_CPP
    #define LANGUAGE_CPP 1
#else
    #define LANGUAGE_NAME "C"
    #undef LANGUAGE_C
    #define LANGUAGE_C 1
#endif


#define C94 (__STDC_VERSION__ == 199409L)
#define C99 (__STDC_VERSION__ == 199901L)
#define C11 (__STDC_VERSION__ == 201112L)
#define C18 (__STDC_VERSION__ == 201710L)

#define CPP98 (__cplusplus == 199711L)
#define CPP11 (__cplusplus == 201103L)
#define CPP14 (__cplusplus == 201402L)
#define CPP17 (__cplusplus == 201703L)
#define CPP20 (__cplusplus == 202002L)


#endif // SYSTEM_ENV_H
/////////////////////////////////// LANGUAGE ///////////////////////////////////
/////////////////////////////////// SYSTEM_ENV_H ///////////////////////////////////

#if LANGUAGE_CPP // C++ support
extern "C" {    // prevent name mangling
#endif         // C++ support


#define VECTOR_DEFAULT_INITIAL_CAPACITY 4

typedef void (*Vector_free_fn)(void *vec_ptr);
typedef size_t (*Vector_calculate_optimal_capacity_fn)(void *vec_ptr);

/**
 * Internal
 * 
 * The header of a vector, which contains metadata about the vector
 * @note The data of the vector is stored in a flexible array member at the end of the struct
 * @note The header is stored in memory before the data of the vector, so that we can access the header from the data pointer
 * @note The header is not exposed to the user, and should not be accessed directly
 * @note I am storing the element size in the header so that i can have a workaround for some functions for the compilers that do not support 'typeof' keyword
 */
typedef struct __Vector_Header {
    size_t element_size;
    size_t length;
    size_t capacity;
    size_t initial_capacity;
    Vector_free_fn free_fn; // Cast the pointer to the vector to the type you want and free it
    Vector_calculate_optimal_capacity_fn calculate_optimal_capacity_fn;
    char data[];
} __Vector_Header;


/**
 * Internal
 * 
 * Returns a pointer to the header of a vector
 * @param vec_ptr [T**]              - A reference to the vector
 * @return        [__Vector_Header*] - A pointer to the header of the vector
 * @throw         [assert]           - If the vector is NULL
 */
__Vector_Header *__vector_get_header(void *vec_ptr);

/**
 * Internal
 * 
 * Resizes the vector if the capacity is not optimal
 * @param vec_ptr [T**]    - A reference to the vector
 * @throw         [assert] - If the vector is NULL
 */
void __vector_resize_if_needed(void *vec_ptr);

/**
 * Internal
 * 
 * The optional parameters for initializing a vector
 * @note This struct is used to pass optional parameters to the __vector_init function, which is called by the Vector_init macro
 * @note This struct is not exposed to the user, and should not be accessed directly
 */
typedef struct __Vector_Init_Params {
    size_t initial_capacity;
    Vector_free_fn free_fn;
    Vector_calculate_optimal_capacity_fn calculate_optimal_capacity_fn;
} __Vector_Init_Params;

/**
 * Internal
 * 
 * Initializes a vector
 * @param element_size       [size_t]               - The size of the vector type
 * @param vector_init_params [__Vector_Init_Params] - The optional parameters for initializing the vector
 * @return                   [T*]                   - The array of data
 * @throw                    [assert]               - If malloc fails
 */
void *__vector_init(size_t element_size, __Vector_Init_Params vector_init_params);

#if !LANGUAGE_CPP
    /**
     * Public
     * 
     * Initializes a vector
     * @param          __T__                         [type]                                 - The type of the vector elements
     * @optional param initial_capacity              [size_t]                               - The initial capacity of the vector
     * @optional param free_fn                       [Vector_free_fn]                       - The free function to free the vector
     * @optional param calculate_optimal_capacity_fn [Vector_calculate_optimal_capacity_fn] - The function that calculates the optimal capacity of the vector
     * @return                                       [T*]                                   - The vector data
     */
    #define Vector_init(__T__, ...) (__T__*)__vector_init(sizeof(__T__), \
        (__Vector_Init_Params) { \
            .initial_capacity = VECTOR_DEFAULT_INITIAL_CAPACITY, \
            .free_fn = NULL, \
            .calculate_optimal_capacity_fn = NULL, \
            __VA_ARGS__ \
        } \
    )
#else // !LANGUAGE_CPP
    /**
     * Public
     * 
     * Initializes a vector
     * @param  __T__ [type] - The type of the vector elements
     * @return       [T*]   - The vector data
     * @note In C++, the optional parameters are not supported. That is because designated initializers in C++ have some limitations that make it impossible to use them in this case.
     * @note The limitations in this case are that the designated initializers must be specified only once. which is not possible in this case because the optional parameters have default values that are specified in the macro. and if the user specifies a value for an optional parameter, it will be specified twice, which is not allowed in C++.
     * @note If you are using C++, you have to use the long route of setting those values after the vector is initialized by calling the appropriate function for each field.
     */
    #define Vector_init(__T__) (__T__*)__vector_init(sizeof(__T__), \
        (__Vector_Init_Params) { \
            .initial_capacity = VECTOR_DEFAULT_INITIAL_CAPACITY, \
            .free_fn = NULL, \
            .calculate_optimal_capacity_fn = NULL, \
        } \
    )
#endif // !LANGUAGE_CPP

/**
 * Public
 * 
 * Returns the size of an element in the vector
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [size_t] - The element size of the vector
 * @throw         [assert] - If the vector is NULL
 */
size_t Vector_get_element_size(void *vec_ptr);

/**
 * Public
 * 
 * Returns the number of elements in a vector
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [size_t] - The number of elements in the vector
 * @throw         [assert] - If the vector is NULL
 */
size_t Vector_get_length(void *vec_ptr);

/**
 * Public
 * 
 * Returns the capacity of the vector
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [size_t] - The capacity of the vector
 * @throw         [assert] - If the vector is NULL
 */
size_t Vector_get_capacity(void *vec_ptr);

/**
 * Public
 * 
 * Returns the initial capacity of a vector
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [size_t] - The initial capacity of the vector
 * @throw         [assert] - If the vector is NULL
 */
size_t Vector_get_initial_capacity(void *vec_ptr);

/**
 * Public
 * 
 * Checks if the vector is full
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [bool]   - True if the vector is full, false otherwise
 * @throw         [assert] - If the vector is NULL
 */
bool Vector_is_full(void *vec_ptr);

/**
 * Public
 * 
 * Returns true if the capacity is less than half full (there is a lot of unused space) and capacity is greater than initial_capacity (the initial_capacity is the minimum capacity)
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [bool]   - True if the vector is underfilled, false otherwise
 * @throw         [assert] - If the vector is NULL
 */
bool Vector_is_underfilled(void *vec_ptr);

/**
 * Public
 * 
 * Checks if the vector is empty
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [bool]   - True if the vector is empty, false otherwise
 * @throw         [assert] - If the vector is NULL
 */
bool Vector_is_empty(void *vec_ptr);

/**
 * Public
 * 
 * Sets the custom initial capacity of the vector and resizes it immediately
 * The default initial capacity is 4
 * @param vec_ptr          [T**]    - A reference to the vector
 * @param initial_capacity [size_t] - The initial capacity of the vector
 * @throw                  [assert] - If the vector is NULL
 */
void Vector_set_initial_capacity(void *vec_ptr, size_t initial_capacity);

/**
 * Public
 * 
 * Sets the custom free function of the vector
 * @param vec_ptr [T**]            - A reference to the vector
 * @param free_fn [Vector_free_fn] - The free function to free the vector
 * @throw         [assert]         - If the vector is NULL
 */
void Vector_set_free_fn(void *vec_ptr, Vector_free_fn free_fn);

/**
 * Public
 * 
 * Sets the custom function that calculates the optimal capacity of the vector
 * @param vec_ptr                       [T**]                                  - A reference to the vector
 * @param calculate_optimal_capacity_fn [Vector_calculate_optimal_capacity_fn] - The function that calculates the optimal capacity of the vector
 * @throw                               [assert]                               - If the vector is NULL
 */
void Vector_set_calculate_optimal_capacity_fn(void *vec_ptr, Vector_calculate_optimal_capacity_fn calculate_optimal_capacity_fn);

/**
 * Public
 * 
 * Destroys and frees the vector
 * @param __vec_ptr__ [T**]    - A reference to the vector
 * @throw             [assert] - If the vector is NULL
 */
#define Vector_destroy(__vec_ptr__) do {                              \
    __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
    if (__header__->free_fn == NULL) {                                \
        free(__header__);                                             \
        (*(__vec_ptr__)) = NULL;                                      \
    } else {                                                          \
        __header__->free_fn((void*)(__vec_ptr__));                    \
    }                                                                 \
} while (0)

#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Returns the index of the value in the vector
     * @param __vec_ptr__            [T**]           - A reference to the vector
     * @param __value__              [size_t]        - The value to get the index of
     * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values (should return true if the values are equal, false otherwise), the first argument is the value in the vector, the second argument is the value given as an argument
     * @return                       [size_t]        - The index of the value in the vector
     * @throw                        [assert]        - If the vector is NULL
     * @throw                        [assert]        - If the value does not exist in the vector
     */
    #define Vector_index_of(__vec_ptr__, __value__, __boolean_comparator__) ({    \
        bool __found__ = false;                                                   \
        size_t __i__ = 0;                                                         \
        for ( ; __i__ < Vector_get_length((__vec_ptr__)); __i__++) {              \
            if ((__boolean_comparator__)((*(__vec_ptr__))[__i__], (__value__))) { \
                __found__ = true;                                                 \
                break;                                                            \
            }                                                                     \
        }                                                                         \
        assertf(__found__, "ERROR: Value not found\n");                           \
        __i__;                                                                    \
    })
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Returns the index of the value in the vector
     * @param __vec_ptr__            [T**]           - A reference to the vector
     * @param __value__              [size_t]        - The value to get the index of
     * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values (should return true if the values are equal, false otherwise), the first argument is the value in the vector, the second argument is the value given as an argument
     * @param __result_ptr__         [size_t*]       - A pointer to the variable to store the index in, if NULL, the result will not be stored but the function will execute normally
     * @throw                        [assert]        - If the vector is NULL
     * @throw                        [assert]        - If the value does not exist in the vector
     */
    #define Vector_index_of(__vec_ptr__, __value__, __boolean_comparator__, __result_ptr__) do { \
        bool __found__ = false;                                                                  \
        size_t __i__ = 0;                                                                        \
        for ( ; __i__ < Vector_get_length((__vec_ptr__)); __i__++) {                             \
            if ((__boolean_comparator__)((*(__vec_ptr__))[__i__], (__value__))) {                \
                __found__ = true;                                                                \
                break;                                                                           \
            }                                                                                    \
        }                                                                                        \
        assertf(__found__, "ERROR: Value not found\n");                                          \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __i__; }                           \
    } while (0)
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS


#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Returns the count of the value in the vector
     * @param __vec_ptr__            [T**]           - A reference to the vector
     * @param __value__              [T]             - The value to get the count of
     * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value given as an argument
     * @return                       [size_t]        - The count of the value in the vector
     * @throw                        [assert]        - If the vector is NULL
     */
    #define Vector_count(__vec_ptr__, __value__, __boolean_comparator__) ({         \
        size_t __count__ = 0;                                                       \
        for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
            if ((__boolean_comparator__)((*(__vec_ptr__))[__i__], (__value__))) {   \
                __count__++;                                                        \
            }                                                                       \
        }                                                                           \
        __count__;                                                                  \
    })
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Returns the count of the value in the vector
     * @param __vec_ptr__            [T**]           - A reference to the vector
     * @param __value__              [T]             - The value to get the count of
     * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value given as an argument
     * @param __result_ptr__         [size_t*]       - A pointer to the variable to store the count in, if NULL, the result will not be stored but the function will execute normally
     * @throw                        [assert]        - If the vector is NULL
     */
    #define Vector_count(__vec_ptr__, __value__, __boolean_comparator__, __result_ptr__) do { \
        size_t __count__ = 0;                                                                 \
        for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) {           \
            if ((__boolean_comparator__)((*(__vec_ptr__))[__i__], (__value__))) {             \
                __count__++;                                                                  \
            }                                                                                 \
        }                                                                                     \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __count__; }                    \
    } while (0)
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

/**
 * Public
 * 
 * Pushes a value to the end of the vector
 * @param __vec_ptr__ [T**]    - A reference to the vector
 * @param __value__   [T]      - The value to push to the vector
 * @throw             [assert] - If the vector is NULL
 * @throw             [assert] - If malloc fails
 */
#define Vector_push(__vec_ptr__, __value__) do {                      \
    __vector_resize_if_needed((__vec_ptr__));                         \
    __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
    assertf(__header__->length < __header__->capacity, "ERROR: Vector is full\n"); \
    (*(__vec_ptr__))[__header__->length++] = (__value__);             \
} while (0)

/**
 * Public
 * 
 * Inserts a value at the specified index in the vector
 * @param __vec_ptr__ [T**]    - A reference to the vector
 * @param __index__   [size_t] - The index to insert the value at
 * @param __value__   [T]      - The value to insert
 * @throw             [assert] - If the vector is NULL
 * @throw             [assert] - If the index is out of bounds
 * @throw             [assert] - If malloc fails
 */
#define Vector_insert_at(__vec_ptr__, __index__, __value__) do {                                                                                           \
    __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));                                                                                      \
    assertf((__index__) >= 0 && (__index__) <= __header__->length, "ERROR: Index: %d out of bounds [%d, %zu]\n", (int)(__index__), 0, __header__->length); \
    __vector_resize_if_needed((__vec_ptr__));                                                                                                              \
    memmove((*(__vec_ptr__)) + (__index__) + 1, (*(__vec_ptr__)) + (__index__), (__header__->length - (__index__)) * __header__->element_size);            \
    (*(__vec_ptr__))[(__index__)] = (__value__);                                                                                                           \
    __header__->length++;                                                                                                                                  \
} while (0)

#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Inserts a value into the vector in sorted order
     * @param __vec_ptr__             [T**]           - A reference to the vector
     * @param __value__               [T]             - The value to insert
     * @param __ordering_comparator__ [int (*)(T, T)] - The ordering comparator function to compare the values
     * @return                        [size_t]        - The index of the value inserted into the vector
     * @throw                         [assert]        - If the vector is NULL
     * @throw                         [assert]        - If malloc fails
     */
    #define Vector_insert_sorted(__vec_ptr__, __value__, __ordering_comparator__) ({      \
        __vector_resize_if_needed((__vec_ptr__));                                         \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));                 \
        size_t __low__ = 0;                                                               \
        size_t __high__ = __header__->length;                                             \
        while (__low__ < __high__) {                                                      \
            size_t __mid__ = (__low__ + __high__) / 2;                                    \
            if ((__ordering_comparator__)((*(__vec_ptr__))[__mid__], (__value__)) >= 0) { \
                __high__ = __mid__;                                                       \
            } else {                                                                      \
                __low__ = __mid__ + 1;                                                    \
            }                                                                             \
        }                                                                                 \
        Vector_insert_at((__vec_ptr__), __low__, (__value__));                            \
        __low__;                                                                          \
    })
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Inserts a value into the vector in sorted order
     * @param __vec_ptr__             [T**]           - A reference to the vector
     * @param __value__               [T]             - The value to insert
     * @param __ordering_comparator__ [int (*)(T, T)] - The ordering comparator function to compare the values
     * @param __result_ptr__          [size_t*]       - A pointer to the variable to store the index of the value inserted into the vector, if NULL, the result will not be stored but the function will execute normally
     * @throw                         [assert]        - If the vector is NULL
     * @throw                         [assert]        - If malloc fails
     */
    #define Vector_insert_sorted(__vec_ptr__, __value__, __ordering_comparator__, __result_ptr__) do { \
        __vector_resize_if_needed((__vec_ptr__));                                                      \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));                              \
        size_t __low__ = 0;                                                                            \
        size_t __high__ = __header__->length;                                                          \
        while (__low__ < __high__) {                                                                   \
            size_t __mid__ = (__low__ + __high__) / 2;                                                 \
            if ((__ordering_comparator__)((*(__vec_ptr__))[__mid__], (__value__)) >= 0) {              \
                __high__ = __mid__;                                                                    \
            } else {                                                                                   \
                __low__ = __mid__ + 1;                                                                 \
            }                                                                                          \
        }                                                                                              \
        Vector_insert_at((__vec_ptr__), __low__, (__value__));                                         \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __low__; }                               \
    } while (0)
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

/**
 * Public
 * 
 * Appends a copy of the values in the second vector to the first vector
 * @param __vec_ptr1__ [T**]    - A reference to the first vector
 * @param __vec_ptr2__ [T**]    - A reference to the second vector
 * @throw              [assert] - If the first vector is NULL
 * @throw              [assert] - If the second vector is NULL
 * @throw              [assert] - If malloc fails
 */
#define Vector_concat(__vec_ptr1__, __vec_ptr2__) do {                           \
    for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr2__)); __i__++) { \
        Vector_push((__vec_ptr1__), (*(__vec_ptr2__))[__i__]);                   \
    }                                                                            \
} while (0)

#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Pops the last value from the vector and returns it
         * @param __vec_ptr__ [T**]    - A reference to the vector
         * @return            [T]      - The value popped from the vector
         * @throw             [assert] - If the vector is NULL
         * @throw             [assert] - If malloc fails
         * @throw             [assert] - If the vector is empty
         */
        #define Vector_pop(__vec_ptr__) ({                                                \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));             \
            assertf(__header__->length > 0, "ERROR: Vector is empty\n");                  \
            typeof(**(__vec_ptr__)) __value__ = (*(__vec_ptr__))[__header__->length - 1]; \
            __header__->length--;                                                         \
            __vector_resize_if_needed((__vec_ptr__));                                     \
            __value__;                                                                    \
        })
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Pops the last value from the vector and returns it
         * @param __vec_ptr__          [T**]    - A reference to the vector
         * @param __vec_element_type__ [type]   - The type of the vector elements
         * @return                     [T]      - The value popped from the vector
         * @throw                      [assert] - If the vector is NULL
         * @throw                      [assert] - If malloc fails
         * @throw                      [assert] - If the vector is empty
         */
        #define Vector_pop(__vec_ptr__, __vec_element_type__) ({                       \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));          \
            assertf(__header__->length > 0, "ERROR: Vector is empty\n");               \
            __vec_element_type__ __value__ = (*(__vec_ptr__))[__header__->length - 1]; \
            __header__->length--;                                                      \
            __vector_resize_if_needed((__vec_ptr__));                                  \
            __value__;                                                                 \
        })
    #endif // COMPILER_SUPPORTS_TYPEOF
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Pops the last value from the vector and returns it
     * @param __vec_ptr__    [T**]    - A reference to the vector
     * @param __result_ptr__ [T*]     - A pointer to the variable to store the popped value in, if NULL, the result will not be stored but the function will execute normally
     * @return               [T]      - The value popped from the vector
     * @throw                [assert] - If the vector is NULL
     * @throw                [assert] - If malloc fails
     * @throw                [assert] - If the vector is empty
     */
    #define Vector_pop(__vec_ptr__, __result_ptr__) do {                    \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));   \
        assertf(__header__->length > 0, "ERROR: Vector is empty\n");        \
        if ((__result_ptr__) != NULL) {                                     \
            (*(__result_ptr__)) = (*(__vec_ptr__))[__header__->length - 1]; \
        }                                                                   \
        __header__->length--;                                               \
        __vector_resize_if_needed((__vec_ptr__));                           \
    } while (0)
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Removes the value at the specified index from the vector
         * @param __vec_ptr__ [T**]    - A reference to the vector
         * @param __index__   [size_t] - The index to remove the value from
         * @return            [T]      - The value removed from the vector
         * @throw             [assert] - If the vector is NULL
         * @throw             [assert] - If malloc fails
         * @throw             [assert] - If the index is out of bounds
         */
        #define Vector_remove_at(__vec_ptr__, __index__) ({                                                                                                           \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));                                                                                         \
            assertf((__index__) >= 0 && (__index__) < __header__->length, "ERROR: Index: %d out of bounds [%d, %zu]\n", (int)(__index__), 0, __header__->length - 1); \
            typeof(**(__vec_ptr__)) __value__ = (*(__vec_ptr__))[(__index__)];                                                                                        \
            memmove((*(__vec_ptr__)) + (__index__), (*(__vec_ptr__)) + (__index__) + 1, (__header__->length - (__index__) - 1) * __header__->element_size);           \
            __header__->length--;                                                                                                                                     \
            __vector_resize_if_needed((__vec_ptr__));                                                                                                                 \
            __value__;                                                                                                                                                \
        })
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Removes the value at the specified index from the vector
         * @param __vec_ptr__          [T**]    - A reference to the vector
         * @param __index__            [size_t] - The index to remove the value from
         * @param __vec_element_type__ [type]   - The type of the vector elements
         * @return                     [T]      - The value removed from the vector
         * @throw                      [assert] - If the vector is NULL
         * @throw                      [assert] - If malloc fails
         * @throw                      [assert] - If the index is out of bounds
         */
        #define Vector_remove_at(__vec_ptr__, __index__, __vec_element_type__) ({                                                                                     \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));                                                                                         \
            assertf((__index__) >= 0 && (__index__) < __header__->length, "ERROR: Index: %d out of bounds [%d, %zu]\n", (int)(__index__), 0, __header__->length - 1); \
            __vec_element_type__ __value__ = (*(__vec_ptr__))[(__index__)];                                                                                           \
            memmove((*(__vec_ptr__)) + (__index__), (*(__vec_ptr__)) + (__index__) + 1, (__header__->length - (__index__) - 1) * __header__->element_size);           \
            __header__->length--;                                                                                                                                     \
            __vector_resize_if_needed((__vec_ptr__));                                                                                                                 \
            __value__;                                                                                                                                                \
        })
    #endif // COMPILER_SUPPORTS_TYPEOF
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Removes the value at the specified index from the vector
     * @param __vec_ptr__    [T**]    - A reference to the vector
     * @param __index__      [size_t] - The index to remove the value from
     * @param __result_ptr__ [T*]     - A pointer to the variable to store the removed value in, if NULL, the result will not be stored but the function will execute normally
     * @return               [T]      - The value removed from the vector
     * @throw                [assert] - If the vector is NULL
     * @throw                [assert] - If malloc fails
     * @throw                [assert] - If the index is out of bounds
     */
    #define Vector_remove_at(__vec_ptr__, __index__, __result_ptr__) do {                                                                                         \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));                                                                                         \
        assertf((__index__) >= 0 && (__index__) < __header__->length, "ERROR: Index: %d out of bounds [%d, %zu]\n", (int)(__index__), 0, __header__->length - 1); \
        if ((__result_ptr__) != NULL) {                                                                                                                           \
            (*(__result_ptr__)) = (*(__vec_ptr__))[(__index__)];                                                                                                  \
        }                                                                                                                                                         \
        memmove((*(__vec_ptr__)) + (__index__), (*(__vec_ptr__)) + (__index__) + 1, (__header__->length - (__index__) - 1) * __header__->element_size);           \
        __header__->length--;                                                                                                                                     \
        __vector_resize_if_needed((__vec_ptr__));                                                                                                                 \
    } while (0)
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Removes the value at the specified index from the vector without preserving the order of the elements
         * @param __vec_ptr__ [T**]    - A reference to the vector
         * @param __index__   [size_t] - The index to remove the value from
         * @return            [T]      - The value removed from the vector
         * @throw             [assert] - If the vector is NULL
         * @throw             [assert] - If malloc fails
         * @throw             [assert] - If the index is out of bounds
         */
        #define Vector_remove_at_unordered(__vec_ptr__, __index__) ({                                                                                                 \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));                                                                                         \
            assertf((__index__) >= 0 && (__index__) < __header__->length, "ERROR: Index: %d out of bounds [%d, %zu]\n", (int)(__index__), 0, __header__->length - 1); \
            typeof(**(__vec_ptr__)) __value__ = (*(__vec_ptr__))[(__index__)];                                                                                        \
            (*(__vec_ptr__))[(__index__)] = (*(__vec_ptr__))[__header__->length - 1];                                                                                 \
            __header__->length--;                                                                                                                                     \
            __vector_resize_if_needed((__vec_ptr__));                                                                                                                 \
            __value__;                                                                                                                                                \
        })
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Removes the value at the specified index from the vector without preserving the order of the elements
         * @param __vec_ptr__          [T**]    - A reference to the vector
         * @param __index__            [size_t] - The index to remove the value from
         * @param __vec_element_type__ [type]   - The type of the vector elements
         * @return                     [T]      - The value removed from the vector
         * @throw                      [assert] - If the vector is NULL
         * @throw                      [assert] - If malloc fails
         * @throw                      [assert] - If the index is out of bounds
         */
        #define Vector_remove_at_unordered(__vec_ptr__, __index__, __vec_element_type__) ({                                                                           \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));                                                                                         \
            assertf((__index__) >= 0 && (__index__) < __header__->length, "ERROR: Index: %d out of bounds [%d, %zu]\n", (int)(__index__), 0, __header__->length - 1); \
            __vec_element_type__ __value__ = (*(__vec_ptr__))[(__index__)];                                                                                           \
            (*(__vec_ptr__))[(__index__)] = (*(__vec_ptr__))[__header__->length - 1];                                                                                 \
            __header__->length--;                                                                                                                                     \
            __vector_resize_if_needed((__vec_ptr__));                                                                                                                 \
            __value__;                                                                                                                                                \
        })
    #endif // COMPILER_SUPPORTS_TYPEOF
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Removes the value at the specified index from the vector without preserving the order of the elements
     * @param __vec_ptr__    [T**]    - A reference to the vector
     * @param __index__      [size_t] - The index to remove the value from
     * @param __result_ptr__ [T*]     - A pointer to the variable to store the removed value in, if NULL, the result will not be stored but the function will execute normally
     * @return               [T]      - The value removed from the vector
     * @throw                [assert] - If the vector is NULL
     * @throw                [assert] - If malloc fails
     * @throw                [assert] - If the index is out of bounds
     */
    #define Vector_remove_at_unordered(__vec_ptr__, __index__, __result_ptr__) do {                                                                               \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));                                                                                         \
        assertf((__index__) >= 0 && (__index__) < __header__->length, "ERROR: Index: %d out of bounds [%d, %zu]\n", (int)(__index__), 0, __header__->length - 1); \
        if ((__result_ptr__) != NULL) {                                                                                                                           \
            (*(__result_ptr__)) = (*(__vec_ptr__))[(__index__)];                                                                                                  \
        }                                                                                                                                                         \
        (*(__vec_ptr__))[(__index__)] = (*(__vec_ptr__))[__header__->length - 1];                                                                                 \
        __header__->length--;                                                                                                                                     \
        __vector_resize_if_needed((__vec_ptr__));                                                                                                                 \
    } while (0)
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Removes the first occurrence of the value from the vector
         * @param __vec_ptr__            [T**]           - A reference to the vector
         * @param __value__              [T]             - The value to remove
         * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value to search for
         * @return                       [size_t]        - The index of the value removed from the vector
         * @throw                        [assert]        - If the vector is NULL
         * @throw                        [assert]        - If malloc fails
         * @throw                        [assert]        - If the value does not exist in the vector
         */
        #define Vector_remove_value(__vec_ptr__, __value__, __boolean_comparator__) ({                \
            size_t __index__ = Vector_index_of((__vec_ptr__), (__value__), (__boolean_comparator__)); \
            Vector_remove_at((__vec_ptr__), __index__);                                               \
            __index__;                                                                                \
        })
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Removes the first occurrence of the value from the vector
         * @param __vec_ptr__            [T**]           - A reference to the vector
         * @param __value__              [T]             - The value to remove
         * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value to search for
         * @param __vec_element_type__   [type]          - The type of the vector elements
         * @return                       [size_t]        - The index of the value removed from the vector
         * @throw                        [assert]        - If the vector is NULL
         * @throw                        [assert]        - If malloc fails
         * @throw                        [assert]        - If the value does not exist in the vector
         */
        #define Vector_remove_value(__vec_ptr__, __value__, __boolean_comparator__, __vec_element_type__) ({ \
            size_t __index__ = Vector_index_of((__vec_ptr__), (__value__), (__boolean_comparator__));        \
            Vector_remove_at((__vec_ptr__), __index__, __vec_element_type__);                                \
            __index__;                                                                                       \
        })
    #endif // COMPILER_SUPPORTS_TYPEOF
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Removes the first occurrence of the value from the vector
         * @param __vec_ptr__            [T**]           - A reference to the vector
         * @param __value__              [T]             - The value to remove
         * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value to search for
         * @param __result_ptr__         [size_t*]       - A pointer to the variable to store the index of the value removed from the vector, if NULL, the result will not be stored but the function will execute normally
         * @throw                        [assert]        - If the vector is NULL
         * @throw                        [assert]        - If malloc fails
         * @throw                        [assert]        - If the value does not exist in the vector
         */
        #define Vector_remove_value(__vec_ptr__, __value__, __boolean_comparator__, __result_ptr__) do { \
            size_t __index__;                                                                            \
            Vector_index_of((__vec_ptr__), (__value__), (__boolean_comparator__), &__index__);           \
            Vector_remove_at((__vec_ptr__), __index__, (typeof(**(__vec_ptr__)) *)NULL); \
            if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __index__; }                           \
        } while (0)
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Removes the first occurrence of the value from the vector
         * @param __vec_ptr__            [T**]           - A reference to the vector
         * @param __value__              [T]             - The value to remove
         * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value to search for
         * @param __result_ptr__         [size_t*]       - A pointer to the variable to store the index of the value removed from the vector, if NULL, the result will not be stored but the function will execute normally
         * @param __vec_element_type__   [type]          - The type of the vector elements
         * @throw                        [assert]        - If the vector is NULL
         * @throw                        [assert]        - If malloc fails
         * @throw                        [assert]        - If the value does not exist in the vector
         */
        #define Vector_remove_value(__vec_ptr__, __value__, __boolean_comparator__, __result_ptr__, __vec_element_type__) do { \
            size_t __index__;                                                                                                  \
            Vector_index_of((__vec_ptr__), (__value__), (__boolean_comparator__), &__index__);                                 \
            Vector_remove_at((__vec_ptr__), __index__, (__vec_element_type__ *)NULL);                          \
            if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __index__; }                                                 \
        } while (0)
    #endif // COMPILER_SUPPORTS_TYPEOF
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Removes the first occurrence of the value from the vector without preserving the order of the elements
         * @param __vec_ptr__            [T**]           - A reference to the vector
         * @param __value__              [T]             - The value to remove
         * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value to search for
         * @return                       [size_t]        - The index of the value removed from the vector
         * @throw                        [assert]        - If the vector is NULL
         * @throw                        [assert]        - If malloc fails
         * @throw                        [assert]        - If the value does not exist in the vector
         */
        #define Vector_remove_value_unordered(__vec_ptr__, __value__, __boolean_comparator__) ({      \
            size_t __index__ = Vector_index_of((__vec_ptr__), (__value__), (__boolean_comparator__)); \
            Vector_remove_at_unordered((__vec_ptr__), __index__);                                     \
            __index__;                                                                                \
        })
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Removes the first occurrence of the value from the vector without preserving the order of the elements
         * @param __vec_ptr__            [T**]           - A reference to the vector
         * @param __value__              [T]             - The value to remove
         * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value to search for
         * @param __vec_element_type__   [type]          - The type of the vector elements
         * @return                       [size_t]        - The index of the value removed from the vector
         * @throw                        [assert]        - If the vector is NULL
         * @throw                        [assert]        - If malloc fails
         * @throw                        [assert]        - If the value does not exist in the vector
         */
        #define Vector_remove_value_unordered(__vec_ptr__, __value__, __boolean_comparator__, __vec_element_type__) ({ \
            size_t __index__ = Vector_index_of((__vec_ptr__), (__value__), (__boolean_comparator__));                  \
            Vector_remove_at_unordered((__vec_ptr__), __index__, __vec_element_type__);                                \
            __index__;                                                                                                 \
        })
    #endif // COMPILER_SUPPORTS_TYPEOF
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Removes the first occurrence of the value from the vector without preserving the order of the elements
         * @param __vec_ptr__            [T**]           - A reference to the vector
         * @param __value__              [T]             - The value to remove
         * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value to search for
         * @param __result_ptr__         [size_t*]       - A pointer to the variable to store the index of the value removed from the vector, if NULL, the result will not be stored but the function will execute normally
         * @throw                        [assert]        - If the vector is NULL
         * @throw                        [assert]        - If malloc fails
         * @throw                        [assert]        - If the value does not exist in the vector
         */
        #define Vector_remove_value_unordered(__vec_ptr__, __value__, __boolean_comparator__, __result_ptr__) do { \
            size_t __index__;                                                                                      \
            Vector_index_of((__vec_ptr__), (__value__), (__boolean_comparator__), &__index__);                     \
            Vector_remove_at_unordered((__vec_ptr__), __index__, (typeof(**(__vec_ptr__)) *)NULL);                 \
            if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __index__; }                                     \
        } while (0)
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Removes the first occurrence of the value from the vector without preserving the order of the elements
         * @param __vec_ptr__            [T**]           - A reference to the vector
         * @param __value__              [T]             - The value to remove
         * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value to search for
         * @param __result_ptr__         [size_t*]       - A pointer to the variable to store the index of the value removed from the vector, if NULL, the result will not be stored but the function will execute normally
         * @param __vec_element_type__   [type]          - The type of the vector elements
         * @throw                        [assert]        - If the vector is NULL
         * @throw                        [assert]        - If malloc fails
         * @throw                        [assert]        - If the value does not exist in the vector
         */
        #define Vector_remove_value_unordered(__vec_ptr__, __value__, __boolean_comparator__, __result_ptr__, __vec_element_type__) do { \
            size_t __index__;                                                                                                            \
            Vector_index_of((__vec_ptr__), (__value__), (__boolean_comparator__), &__index__);                                           \
            Vector_remove_at_unordered((__vec_ptr__), __index__, (__vec_element_type__ *)NULL);                                          \
            if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __index__; }                                                           \
        } while (0)
    #endif // COMPILER_SUPPORTS_TYPEOF
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS


/**
 * Public
 * 
 * Clears all the elements in the vector
 * @param __vec_ptr__ [T**]    - A reference to the vector
 * @throw             [assert] - If the vector is NULL
 */
#define Vector_clear(__vec_ptr__) do {                                \
    __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
    __header__->length = 0;                                           \
    __vector_resize_if_needed((__vec_ptr__));                         \
} while (0)


// the result is casted to void* to avoid the casting warning
#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Returns a shallow copy of the vector
     * @param __vec_ptr__ [T**]    - A reference to the vector
     * @return            [T*]     - The copied vector
     * @throw             [assert] - If the vector is NULL
     * @throw             [assert] - If malloc fails
     * @note the returned result is a shallow copy, if the vector contains pointers to objects, the objects will not be copied
     */
    #define Vector_copy(__vec_ptr__) ({                                                                                                        \
        __Vector_Header *__old_vec__ = __vector_get_header((__vec_ptr__));                                                                     \
        __Vector_Header *__new_vec__ = (__Vector_Header *)malloc(sizeof(__Vector_Header) + __old_vec__->capacity * __old_vec__->element_size); \
        assertf(__new_vec__ != NULL, "ERROR: Allocation failed\n");                                                                            \
        memcpy(__new_vec__, __old_vec__, sizeof(__Vector_Header) + __old_vec__->length * __old_vec__->element_size);                           \
        (void*)__new_vec__->data;                                                                                                              \
    })
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Returns a shallow copy of the vector
     * @param __old_vec_ptr__ [T**]    - A reference to the vector
     * @param __new_vec_ptr__ [T**]    - A reference to the copied vector
     * @throw                 [assert] - If the vector is NULL
     * @throw                 [assert] - If malloc fails
     * @note the returned result is a shallow copy, if the vector contains pointers to objects, the objects will not be copied
     */
    #define Vector_copy(__old_vec_ptr__, __new_vec_ptr__) do {                                                                                 \
        __Vector_Header *__old_vec__ = __vector_get_header((__old_vec_ptr__));                                                                 \
        __Vector_Header *__new_vec__ = (__Vector_Header *)malloc(sizeof(__Vector_Header) + __old_vec__->capacity * __old_vec__->element_size); \
        assertf(__new_vec__ != NULL, "ERROR: Allocation failed\n");                                                                            \
        memcpy(__new_vec__, __old_vec__, sizeof(__Vector_Header) + __old_vec__->length * __old_vec__->element_size);                           \
        (*(__new_vec_ptr__)) = (void*)__new_vec__->data;                                                                                       \
    } while (0)
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

#if COMPILER_SUPPORTS_TYPEOF
    /**
     * Public
     * 
     * Reverses the vector in place
     * @param __vec_ptr__ [T**]    - A reference to the vector
     * @throw             [assert] - If the vector is NULL
     */
    #define Vector_reverse(__vec_ptr__) do {                                            \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));               \
        typeof(**(__vec_ptr__)) __temp__;                                               \
        for (size_t __i__ = 0; __i__ < __header__->length / 2; __i__++) {               \
            __temp__ = (*(__vec_ptr__))[__i__];                                         \
            (*(__vec_ptr__))[__i__] = (*(__vec_ptr__))[__header__->length - __i__ - 1]; \
            (*(__vec_ptr__))[__header__->length - __i__ - 1] = __temp__;                \
        }                                                                               \
    } while (0)
#else // COMPILER_SUPPORTS_TYPEOF
    /**
     * Public
     * 
     * Reverses the vector in place
     * @param __vec_ptr__          [T**]    - A reference to the vector
     * @param __vec_element_type__ [type]   - A reference to the vector
     * @throw                      [assert] - If the vector is NULL
     */
    #define Vector_reverse(__vec_ptr__, __vec_element_type__) do {                      \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));               \
        __vec_element_type__ __temp__;                                                  \
        for (size_t __i__ = 0; __i__ < __header__->length / 2; __i__++) {               \
            __temp__ = (*(__vec_ptr__))[__i__];                                         \
            (*(__vec_ptr__))[__i__] = (*(__vec_ptr__))[__header__->length - __i__ - 1]; \
            (*(__vec_ptr__))[__header__->length - __i__ - 1] = __temp__;                \
        }                                                                               \
    } while (0)
#endif // COMPILER_SUPPORTS_TYPEOF

#if COMPILER_SUPPORTS_TYPEOF
    #define __merge__(__arr__, __left_size__, __mid__, __right_size__, __ordering_comparator__) do { \
        int __i__, __j__, __k__;                                                                     \
        int __n1__ = (__mid__) - (__left_size__) + 1;                                                \
        int __n2__ =  (__right_size__) - (__mid__);                                                  \
        typeof(*(__arr__)) *__left_array__ = (typeof(__arr__))malloc(__n1__ * sizeof(*(__arr__)));   \
        typeof(*(__arr__)) *__right_array__ = (typeof(__arr__))malloc(__n2__ * sizeof(*(__arr__)));  \
        for (__i__ = 0; __i__ < __n1__; __i__++) {                                                   \
            __left_array__[__i__] = (__arr__)[(__left_size__) + __i__];                              \
        }                                                                                            \
        for (__j__ = 0; __j__ < __n2__; __j__++) {                                                   \
            __right_array__[__j__] = (__arr__)[(__mid__) + 1 + __j__];                               \
        }                                                                                            \
        __i__ = 0; __j__ = 0; __k__ = (__left_size__);                                               \
        while (__i__ < __n1__ && __j__ < __n2__) {                                                   \
            if ((__ordering_comparator__)(__left_array__[__i__], __right_array__[__j__]) <= 0) {     \
                (__arr__)[__k__++] = __left_array__[__i__++];                                        \
            } else {                                                                                 \
                (__arr__)[__k__++] = __right_array__[__j__++];                                       \
            }                                                                                        \
        }                                                                                            \
        while (__i__ < __n1__) {                                                                     \
            (__arr__)[__k__++] = __left_array__[__i__++];                                            \
        }                                                                                            \
        while (__j__ < __n2__) {                                                                     \
            (__arr__)[__k__++] = __right_array__[__j__++];                                           \
        }                                                                                            \
        free(__left_array__);                                                                        \
        free(__right_array__);                                                                       \
    } while(0)

    #define __merge_sort__(__arr__, __n__, __ordering_comparator__) do {                                                                 \
        int __curr_size__, __left_start__;                                                                                               \
        for (__curr_size__ = 1; __curr_size__ <= (__n__) - 1; __curr_size__ = 2 * __curr_size__) {                                       \
            for (__left_start__ = 0; __left_start__ < (__n__) - 1; __left_start__ += 2 * __curr_size__) {                                \
                int __mid__ = __left_start__ + __curr_size__ < (__n__) ? __left_start__ + __curr_size__ - 1 : (__n__) - 1;               \
                int __right_end__ = __left_start__ + 2 * __curr_size__ < (__n__) ? __left_start__ + 2 * __curr_size__ - 1 : (__n__) - 1; \
                __merge__((__arr__), __left_start__, __mid__, __right_end__, (__ordering_comparator__));                                 \
            }                                                                                                                            \
        }                                                                                                                                \
    } while(0)

    /**
     * Public
     * 
     * Sorts the vector in place
     * @param __vec_ptr__             [T**]           - A reference to the vector
     * @param __ordering_comparator__ [int (*)(T, T)] - The ordering comparator function to compare the values, should return a positive number if the first value is greater than the second value, a negative number if the first value is less than the second value, and 0 if the values are equal
     * @throw                         [assert]        - If the vector is NULL
     */
    #define Vector_sort(__vec_ptr__, __ordering_comparator__) do {                       \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));                \
        __merge_sort__((*(__vec_ptr__)), __header__->length, (__ordering_comparator__)); \
    } while (0)
#else // COMPILER_SUPPORTS_TYPEOF
    #define __merge__(__arr__, __left_size__, __mid__, __right_size__, __ordering_comparator__, __vec_element_type__) do { \
        int __i__, __j__, __k__;                                                                                           \
        int __n1__ = (__mid__) - (__left_size__) + 1;                                                                      \
        int __n2__ =  (__right_size__) - (__mid__);                                                                        \
        __vec_element_type__ *__left_array__ = (__vec_element_type__ *)malloc(__n1__ * sizeof(*(__arr__)));                \
        __vec_element_type__ *__right_array__ = (__vec_element_type__ *)malloc(__n2__ * sizeof(*(__arr__)));               \
        for (__i__ = 0; __i__ < __n1__; __i__++) {                                                                         \
            __left_array__[__i__] = (__arr__)[(__left_size__) + __i__];                                                    \
        }                                                                                                                  \
        for (__j__ = 0; __j__ < __n2__; __j__++) {                                                                         \
            __right_array__[__j__] = (__arr__)[(__mid__) + 1 + __j__];                                                     \
        }                                                                                                                  \
        __i__ = 0; __j__ = 0; __k__ = (__left_size__);                                                                     \
        while (__i__ < __n1__ && __j__ < __n2__) {                                                                         \
            if ((__ordering_comparator__)(__left_array__[__i__], __right_array__[__j__]) <= 0) {                           \
                (__arr__)[__k__++] = __left_array__[__i__++];                                                              \
            } else {                                                                                                       \
                (__arr__)[__k__++] = __right_array__[__j__++];                                                             \
            }                                                                                                              \
        }                                                                                                                  \
        while (__i__ < __n1__) {                                                                                           \
            (__arr__)[__k__++] = __left_array__[__i__++];                                                                  \
        }                                                                                                                  \
        while (__j__ < __n2__) {                                                                                           \
            (__arr__)[__k__++] = __right_array__[__j__++];                                                                 \
        }                                                                                                                  \
        free(__left_array__);                                                                                              \
        free(__right_array__);                                                                                             \
    } while(0)

    #define __merge_sort__(__arr__, __n__, __ordering_comparator__, __vec_element_type__) do {                                           \
        int __curr_size__, __left_start__;                                                                                               \
        for (__curr_size__ = 1; __curr_size__ <= (__n__) - 1; __curr_size__ = 2 * __curr_size__) {                                       \
            for (__left_start__ = 0; __left_start__ < (__n__) - 1; __left_start__ += 2 * __curr_size__) {                                \
                int __mid__ = __left_start__ + __curr_size__ < (__n__) ? __left_start__ + __curr_size__ - 1 : (__n__) - 1;               \
                int __right_end__ = __left_start__ + 2 * __curr_size__ < (__n__) ? __left_start__ + 2 * __curr_size__ - 1 : (__n__) - 1; \
                __merge__((__arr__), __left_start__, __mid__, __right_end__, (__ordering_comparator__), __vec_element_type__);           \
            }                                                                                                                            \
        }                                                                                                                                \
    } while(0)

    /**
     * Public
     * 
     * Sorts the vector in place
     * @param __vec_ptr__             [T**]           - A reference to the vector
     * @param __ordering_comparator__ [int (*)(T, T)] - The ordering comparator function to compare the values, should return a positive number if the first value is greater than the second value, a negative number if the first value is less than the second value, and 0 if the values are equal
     * @param __vec_element_type__    [type]          - The type of the elements in the vector
     * @throw                         [assert]        - If the vector is NULL
     */
    #define Vector_sort(__vec_ptr__, __ordering_comparator__, __vec_element_type__) do {                       \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));                                      \
        __merge_sort__((*(__vec_ptr__)), __header__->length, (__ordering_comparator__), __vec_element_type__); \
    } while (0)
#endif // COMPILER_SUPPORTS_TYPEOF

#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Filters the vector based on a filter function
         * @param __vec_ptr__ [T**]         - A reference to the vector
         * @param __filter__  [bool (*)(T)] - The filter function to filter the values
         * @return            [T*]          - A new filtered vector
         * @throw             [assert]      - If the vector is NULL
         * @throw             [assert]      - If malloc fails
         */
        #define Vector_filter(__vec_ptr__, __filter__) ({                               \
            typeof(*(__vec_ptr__)) __new_vec__ = Vector_init(typeof(**(__vec_ptr__)));  \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
                if ((__filter__)((*(__vec_ptr__))[__i__])) {                            \
                    Vector_push(&__new_vec__, (*(__vec_ptr__))[__i__]);                 \
                }                                                                       \
            }                                                                           \
            __new_vec__;                                                                \
        })
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Filters the vector based on a filter function
         * @param __vec_ptr__          [T**]         - A reference to the vector
         * @param __filter__           [bool (*)(T)] - The filter function to filter the values
         * @param __vec_element_type__ [type]        - The type of the elements of the returned vector
         * @return                     [T*]          - A new filtered vector
         * @throw                      [assert]      - If the vector is NULL
         * @throw                      [assert]      - If malloc fails
         */
        #define Vector_filter(__vec_ptr__, __filter__, __vec_element_type__) ({         \
            __vec_element_type__ *__new_vec__ = Vector_init(__vec_element_type__);      \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
                if ((__filter__)((*(__vec_ptr__))[__i__])) {                            \
                    Vector_push(&__new_vec__, (*(__vec_ptr__))[__i__]);                 \
                }                                                                       \
            }                                                                           \
            __new_vec__;                                                                \
        })
    #endif // COMPILER_SUPPORTS_TYPEOF
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Filters the vector based on a filter function
         * @param __vec_ptr__     [T**]         - A reference to the vector
         * @param __filter__      [bool (*)(T)] - The filter function to filter the values
         * @param __new_vec_ptr__ [T**]         - A reference to the pointer to the new vector
         * @throw                 [assert]      - If the vector is NULL
         * @throw                 [assert]      - If malloc fails
         */
        #define Vector_filter(__vec_ptr__, __filter__, __new_vec_ptr__) do {            \
            (*(__new_vec_ptr__)) = Vector_init(typeof(**(__vec_ptr__)));                \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
                if ((__filter__)((*(__vec_ptr__))[__i__])) {                            \
                    Vector_push((__new_vec_ptr__), (*(__vec_ptr__))[__i__]);            \
                }                                                                       \
            }                                                                           \
        } while (0)
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Filters the vector based on a filter function
         * @param __vec_ptr__          [T**]         - A reference to the vector
         * @param __filter__           [bool (*)(T)] - The filter function to filter the values
         * @param __new_vec_ptr__      [T**]         - A reference to the pointer to the new vector
         * @param __vec_element_type__ [type]        - The type of the elements of the returned vector
         * @throw                      [assert]      - If the vector is NULL
         * @throw                      [assert]      - If malloc fails
         */
        #define Vector_filter(__vec_ptr__, __filter__, __new_vec_ptr__, __vec_element_type__) do { \
            (*(__new_vec_ptr__)) = Vector_init(__vec_element_type__);                              \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) {            \
                if ((__filter__)((*(__vec_ptr__))[__i__])) {                                       \
                    Vector_push((__new_vec_ptr__), (*(__vec_ptr__))[__i__]);                       \
                }                                                                                  \
            }                                                                                      \
        } while (0)
    #endif // COMPILER_SUPPORTS_TYPEOF
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

/**
 * Public
 * 
 * Applies a function to each value in the vector
 * @param __vec_ptr__ [T**]          - A reference to the vector
 * @param __func__    [void (*)(T*)] - The function to apply to each value, it takes a pointer to the value
 * @throw             [assert]       - If the vector is NULL
 */
#define Vector_foreach(__vec_ptr__, __func__) do {                              \
    for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
        (__func__)(&((*(__vec_ptr__))[__i__]));                                 \
    }                                                                           \
} while (0)

#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Maps a function to each value in the vector and returns a new vector
     * @param __vec_ptr__              [T**]      - A reference to the vector
     * @param __mapper__               [T (*)(T)] - The mapper function to map to each value
     * @param __new_vec_element_type__ [type]     - The type of the elements of the returned vector
     * @return                         [T*]       - A new mapped vector
     * @throw                          [assert]   - If the vector is NULL
     * @throw                          [assert]   - If malloc fails
     */
    // Note: we accept __new_vec_element_type__ even in case of COMPILER_SUPPORTS_TYPEOF because it is required for the case where the mapper function returns a different type than the original vector's element type.
    #define Vector_map(__vec_ptr__, __mapper__, __new_vec_element_type__) ({           \
        __new_vec_element_type__ *__new_vec__ = Vector_init(__new_vec_element_type__); \
        for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) {    \
            Vector_push(&__new_vec__, (__mapper__)((*(__vec_ptr__))[__i__]));          \
        }                                                                              \
        __new_vec__;                                                                   \
    })
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Maps a function to each value in the vector and returns a new vector
         * @param __vec_ptr__     [T**]      - A reference to the vector
         * @param __mapper__      [T (*)(T)] - The mapper function to map to each value
         * @param __new_vec_ptr__ [T**]      - A reference to the new vector
         * @throw                 [assert]   - If the vector is NULL
         * @throw                 [assert]   - If malloc fails
         */
        #define Vector_map(__vec_ptr__, __mapper__, __new_vec_ptr__) do {               \
            (*(__new_vec_ptr__)) = Vector_init(typeof(**(__new_vec_ptr__)));            \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
                Vector_push((__new_vec_ptr__), (__mapper__)((*(__vec_ptr__))[__i__]));  \
            }                                                                           \
        } while (0)
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Maps a function to each value in the vector and returns a new vector
         * @param __vec_ptr__              [T**]      - A reference to the vector
         * @param __mapper__               [T (*)(T)] - The mapper function to map to each value
         * @param __new_vec_ptr__          [T**]      - A reference to the new vector
         * @param __new_vec_element_type__ [type]     - The type of the elements of the returned vector
         * @throw                          [assert]   - If the vector is NULL
         * @throw                          [assert]   - If malloc fails
         */
        #define Vector_map(__vec_ptr__, __mapper__, __new_vec_ptr__, __new_vec_element_type__) do { \
            (*(__new_vec_ptr__)) = Vector_init(__new_vec_element_type__);                           \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) {             \
                Vector_push((__new_vec_ptr__), (__mapper__)((*(__vec_ptr__))[__i__]));              \
            }                                                                                       \
        } while (0)
    #endif // COMPILER_SUPPORTS_TYPEOF
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Reduces the vector to a single value
         * @param __vec_ptr__       [T**]         - A reference to the vector
         * @param __reducer__       [U (*)(U, T)] - The reducer function to reduce the values
         * @param __initial_value__ [U]           - The initial value to start the reduction
         * @return                  [U]           - The reduced value
         * @throw                   [assert]      - If the vector is NULL
         */
        #define Vector_reduce(__vec_ptr__, __reducer__, __initial_value__) ({           \
            typeof((__initial_value__)) accumulator = (__initial_value__);              \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
                accumulator = (__reducer__)(accumulator, (*(__vec_ptr__))[__i__]);      \
            }                                                                           \
            accumulator;                                                                \
        })
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Reduces the vector to a single value
         * @param __vec_ptr__          [T**]         - A reference to the vector
         * @param __reducer__          [T (*)(U, T)] - The reducer function to reduce the values
         * @param __initial_value__    [U]           - The initial value to start the reduction
         * @param __accumulator_type__ [type]        - The type of the accumulator
         * @return                     [U]           - The reduced value
         * @throw                      [assert]      - If the vector is NULL
         */
        #define Vector_reduce(__vec_ptr__, __reducer__, __initial_value__, __accumulator_type__) ({ \
            __accumulator_type__ accumulator = (__initial_value__);                                 \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) {             \
                accumulator = (__reducer__)(accumulator, (*(__vec_ptr__))[__i__]);                  \
            }                                                                                       \
            accumulator;                                                                            \
        })
    #endif // COMPILER_SUPPORTS_TYPEOF
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Reduces the vector to a single value
         * @param __vec_ptr__       [T**]         - A reference to the vector
         * @param __reducer__       [T (*)(U, T)] - The reducer function to reduce the values
         * @param __initial_value__ [U]           - The initial value to start the reduction
         * @param __result_ptr__    [U*]          - A reference to the variable to store the result in, if NULL, the result will not be stored but the function will execute normally
         * @throw                   [assert]      - If the vector is NULL
         */
        #define Vector_reduce(__vec_ptr__, __reducer__, __initial_value__, __result_ptr__) do { \
            typeof((__initial_value__)) __accumulator__ = (__initial_value__);                  \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) {         \
                __accumulator__ = (__reducer__)(__accumulator__, (*(__vec_ptr__))[__i__]);      \
            }                                                                                   \
            if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __accumulator__; }            \
        } while (0)
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Reduces the vector to a single value
         * @param __vec_ptr__          [T**]         - A reference to the vector
         * @param __reducer__          [T (*)(U, T)] - The reducer function to reduce the values
         * @param __initial_value__    [U]           - The initial value to start the reduction
         * @param __result_ptr__       [U*]          - A reference to the variable to store the result in, if NULL, the result will not be stored but the function will execute normally
         * @param __accumulator_type__ [type]        - The type of the accumulator
         * @throw                      [assert]      - If the vector is NULL
         */
        #define Vector_reduce(__vec_ptr__, __reducer__, __initial_value__, __result_ptr__, __accumulator_type__) do { \
            __accumulator_type__ __accumulator__ = (__initial_value__);                                               \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) {                               \
                __accumulator__ = (__reducer__)(__accumulator__, (*(__vec_ptr__))[__i__]);                            \
            }                                                                                                         \
            if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __accumulator__; }                                  \
        } while (0)
    #endif // COMPILER_SUPPORTS_TYPEOF
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Checks if any value in the vector satisfies the function
     * @param __vec_ptr__ [T**]         - A reference to the vector
     * @param __func__    [bool (*)(T)] - The function to check if any value satisfies
     * @return            [bool]        - True if any value satisfies the function, false otherwise
     * @throw             [assert]      - If the vector is NULL
     */
    #define Vector_any(__vec_ptr__, __func__) ({                                    \
        bool __any__ = false;                                                       \
        for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
            if ((__func__)((*(__vec_ptr__))[__i__])) {                              \
                __any__ = true;                                                     \
                break;                                                              \
            }                                                                       \
        }                                                                           \
        __any__;                                                                    \
    })
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Checks if any value in the vector satisfies the function
     * @param __vec_ptr__    [T**]         - A reference to the vector
     * @param __func__       [bool (*)(T)] - The function to check if any value satisfies
     * @param __result_ptr__ [bool*]       - A reference to the variable to store the result in, if NULL, the result will not be stored but the function will execute normally
     * @throw                [assert]      - If the vector is NULL
     */
    #define Vector_any(__vec_ptr__, __func__, __result_ptr__) do {                  \
        bool __any__ = false;                                                       \
        for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
            if ((__func__)((*(__vec_ptr__))[__i__])) {                              \
                __any__ = true;                                                     \
                break;                                                              \
            }                                                                       \
        }                                                                           \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __any__; }            \
    } while (0)
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Checks if all values in the vector satisfy the function
     * @param __vec_ptr__ [T**]         - A reference to the vector
     * @param __func__    [bool (*)(T)] - The function to check if all values satisfy
     * @return            [bool]        - True if all values satisfy the function, false otherwise
     * @throw             [assert]      - If the vector is NULL
     */
    #define Vector_all(__vec_ptr__, __func__) ({                                    \
        bool __all__ = true;                                                        \
        for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
            if (!(__func__)((*(__vec_ptr__))[__i__])) {                             \
                __all__ = false;                                                    \
                break;                                                              \
            }                                                                       \
        }                                                                           \
        __all__;                                                                    \
    })
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Checks if all values in the vector satisfy the function
     * @param __vec_ptr__    [T**]         - A reference to the vector
     * @param __func__       [bool (*)(T)] - The function to check if all values satisfy
     * @param __result_ptr__ [bool*]       - A reference to the variable to store the result in, if NULL, the result will not be stored but the function will execute normally
     * @throw                [assert]      - If the vector is NULL
     */
    #define Vector_all(__vec_ptr__, __func__, __result_ptr__) do {                  \
        bool __all__ = true;                                                        \
        for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
            if (!(__func__)((*(__vec_ptr__))[__i__])) {                             \
                __all__ = false;                                                    \
                break;                                                              \
            }                                                                       \
        }                                                                           \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __all__; }            \
    } while (0)
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Returns a slice of the vector from the start index to the end index with the specified step
         * @param __vec_ptr__ [T**]    - A reference to the vector
         * @param __start__   [size_t] - The start index of the slice
         * @param __end__     [size_t] - The end index of the slice
         * @param __step__    [size_t] - The step of the slice
         * @return            [T*]     - The sliced vector
         * @throw             [assert] - If the vector is NULL
         * @throw             [assert] - If the start index is out of bounds
         * @throw             [assert] - If the end index is out of bounds
         * @throw             [assert] - If the step is less than or equal to 0
         */
        #define Vector_slice(__vec_ptr__, __start__, __end__, __step__) ({                                                                                                 \
            typeof(*(__vec_ptr__)) __new_vec__ = Vector_init(typeof(**(__vec_ptr__)));                                                                                     \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));                                                                                              \
            assertf((__start__) >= 0 && (__start__) <  __header__->length, "ERROR: Start index: %d out of bounds [%d, %zu]\n", (int)__start__, 0, __header__->length - 1); \
            assertf(( __end__ ) >= 0 && ( __end__ ) <= __header__->length, "ERROR: End index: %d out of bounds [%d, %zu]\n"  , (int)__end__  , 0, __header__->length);     \
            assertf((__step__) > 0, "ERROR: Step: %d is less than 1\n", __step__);                                                                                         \
            for (size_t __i__ = (__start__); __i__ < (__end__); __i__ += (__step__)) {                                                                                     \
                Vector_push(&__new_vec__, (*(__vec_ptr__))[__i__]);                                                                                                        \
            }                                                                                                                                                              \
            __new_vec__;                                                                                                                                                   \
        })
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Returns a slice of the vector from the start index to the end index with the specified step
         * @param __vec_ptr__          [T**]         - A reference to the vector
         * @param __start__            [size_t]      - The start index of the slice
         * @param __end__              [size_t]      - The end index of the slice
         * @param __step__             [size_t]      - The step of the slice
         * @param __vec_element_type__ [type]        - The type of the elements of the returned vector
         * @return                     [T*]          - The sliced vector
         * @throw                      [assert]      - If the vector is NULL
         * @throw                      [assert]      - If the start index is out of bounds
         * @throw                      [assert]      - If the end index is out of bounds
         * @throw                      [assert]      - If the step is less than or equal to 0
         */
        #define Vector_slice(__vec_ptr__, __start__, __end__, __step__, __vec_element_type__) ({                                                                           \
            __vec_element_type__ *__new_vec__ = Vector_init(__vec_element_type__);                                                                                         \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));                                                                                              \
            assertf((__start__) >= 0 && (__start__) <  __header__->length, "ERROR: Start index: %d out of bounds [%d, %zu]\n", (int)__start__, 0, __header__->length - 1); \
            assertf(( __end__ ) >= 0 && ( __end__ ) <= __header__->length, "ERROR: End index: %d out of bounds [%d, %zu]\n"  , (int)__end__  , 0, __header__->length);     \
            assertf((__step__) > 0, "ERROR: Step: %d is less than 1\n", __step__);                                                                                         \
            for (size_t __i__ = (__start__); __i__ < (__end__); __i__ += (__step__)) {                                                                                     \
                Vector_push(&__new_vec__, (*(__vec_ptr__))[__i__]);                                                                                                        \
            }                                                                                                                                                              \
            __new_vec__;                                                                                                                                                   \
        })
    #endif // COMPILER_SUPPORTS_TYPEOF
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Returns a slice of the vector from the start index to the end index with the specified step
         * @param __vec_ptr__     [T**]         - A reference to the vector
         * @param __start__       [size_t]      - The start index of the slice
         * @param __end__         [size_t]      - The end index of the slice
         * @param __step__        [size_t]      - The step of the slice
         * @param __new_vec_ptr__ [T**]         - A reference to the new vector
         * @return                [T*]          - The sliced vector
         * @throw                 [assert]      - If the vector is NULL
         * @throw                 [assert]      - If the start index is out of bounds
         * @throw                 [assert]      - If the end index is out of bounds
         * @throw                 [assert]      - If the step is less than or equal to 0
         */
        #define Vector_slice(__vec_ptr__, __start__, __end__, __step__, __new_vec_ptr__) do {                                                                              \
            (*(__new_vec_ptr__)) = Vector_init(typeof(**(__vec_ptr__)));                                                                                                   \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));                                                                                              \
            assertf((__start__) >= 0 && (__start__) <  __header__->length, "ERROR: Start index: %d out of bounds [%d, %zu]\n", (int)__start__, 0, __header__->length - 1); \
            assertf(( __end__ ) >= 0 && ( __end__ ) <= __header__->length, "ERROR: End index: %d out of bounds [%d, %zu]\n"  , (int)__end__  , 0, __header__->length);     \
            assertf((__step__) > 0, "ERROR: Step: %d is less than 1\n", __step__);                                                                                         \
            for (size_t __i__ = (__start__); __i__ < (__end__); __i__ += (__step__)) {                                                                                     \
                Vector_push((__new_vec_ptr__), (*(__vec_ptr__))[__i__]);                                                                                                   \
            }                                                                                                                                                              \
        } while (0)
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Returns a slice of the vector from the start index to the end index with the specified step
         * @param __vec_ptr__          [T**]         - A reference to the vector
         * @param __start__            [size_t]      - The start index of the slice
         * @param __end__              [size_t]      - The end index of the slice
         * @param __step__             [size_t]      - The step of the slice
         * @param __new_vec_ptr__      [T**]         - A reference to the new vector
         * @param __vec_element_type__ [type]        - The type of the elements of the returned vector
         * @return                     [T*]          - The sliced vector
         * @throw                      [assert]      - If the vector is NULL
         * @throw                      [assert]      - If the start index is out of bounds
         * @throw                      [assert]      - If the end index is out of bounds
         * @throw                      [assert]      - If the step is less than or equal to 0
         */
        #define Vector_slice(__vec_ptr__, __start__, __end__, __step__, __new_vec_ptr__, __vec_element_type__) do {                                                        \
            (*(__new_vec_ptr__)) = Vector_init(__vec_element_type__);                                                                                                      \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__));                                                                                              \
            assertf((__start__) >= 0 && (__start__) <  __header__->length, "ERROR: Start index: %d out of bounds [%d, %zu]\n", (int)__start__, 0, __header__->length - 1); \
            assertf(( __end__ ) >= 0 && ( __end__ ) <= __header__->length, "ERROR: End index: %d out of bounds [%d, %zu]\n"  , (int)__end__  , 0, __header__->length);     \
            assertf((__step__) > 0, "ERROR: Step: %d is less than 1\n", __step__);                                                                                         \
            for (size_t __i__ = (__start__); __i__ < (__end__); __i__ += (__step__)) {                                                                                     \
                Vector_push((__new_vec_ptr__), (*(__vec_ptr__))[__i__]);                                                                                                   \
            }                                                                                                                                                              \
        } while (0)
    #endif // COMPILER_SUPPORTS_TYPEOF
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS



// Implementation
#ifdef VECTOR_IMPLEMENTATION

    __Vector_Header *__vector_get_header(void *vec_ptr) {
        // I use assert here instead of assertf because i want it to be inlined as much as possible (i havent' actually tested anything, i just assumed)
        void **temp_ptr = (void **)vec_ptr;
        assertF(*temp_ptr != NULL, "ERROR: Vector is NULL\n");
        return (__Vector_Header *)(((char *)*temp_ptr) - sizeof(__Vector_Header));
    }

    static void *__vector_realloc(void *vec_ptr, size_t new_capacity) {
        __Vector_Header *old_vec = __vector_get_header(vec_ptr);
        __Vector_Header *new_vec = (__Vector_Header *)malloc(sizeof(__Vector_Header) + new_capacity * old_vec->element_size);
        assertf(new_vec != NULL, "ERROR: Memory allocation failed\n");
        memcpy(new_vec, old_vec, sizeof(__Vector_Header) + old_vec->length * old_vec->element_size);
        new_vec->capacity = new_capacity;    
        free(old_vec);
        return new_vec->data;
    }

    #if COMPILER_SUPPORTS_BUILTIN_CLZ
        static size_t __vector_calculate_basic_optimal_capacity(void *vec_ptr) {
            __Vector_Header *header = __vector_get_header(vec_ptr);
            if (header->length < header->initial_capacity) { return header->initial_capacity; }
            size_t optimal_capacity = header->initial_capacity << (__builtin_clzl(header->initial_capacity) - __builtin_clzl(header->length));
            return optimal_capacity <= header->length ? optimal_capacity << 1 : optimal_capacity;
        }
    #else // COMPILER_SUPPORTS_BUILTIN_CLZ
        static size_t __vector_calculate_basic_optimal_capacity(void *vec_ptr) {
            __Vector_Header *header = __vector_get_header(vec_ptr);
            if (header->length < header->initial_capacity) { return header->initial_capacity; }
            size_t optimal_capacity = header->initial_capacity;
            while (optimal_capacity <= header->length) { optimal_capacity <<= 1; }
            return optimal_capacity;
        }
    #endif // COMPILER_SUPPORTS_BUILTIN_CLZ

    void __vector_resize_if_needed(void *vec_ptr) {
        __Vector_Header *header = __vector_get_header(vec_ptr);
        size_t optimal_capacity = header->calculate_optimal_capacity_fn == NULL ? __vector_calculate_basic_optimal_capacity(vec_ptr) : header->calculate_optimal_capacity_fn(vec_ptr);
        if (optimal_capacity != header->capacity) {
            *(void**)vec_ptr = __vector_realloc(vec_ptr, optimal_capacity);
        }
    }

    void *__vector_init(size_t element_size, __Vector_Init_Params vector_init_params) {
        __Vector_Header *header = (__Vector_Header *)malloc(sizeof(__Vector_Header) + element_size * vector_init_params.initial_capacity);
        assertf(header != NULL, "ERROR: Memory allocation failed\n");
        header->element_size = element_size;
        header->length = 0;
        header->capacity = vector_init_params.initial_capacity;
        header->initial_capacity = vector_init_params.initial_capacity;
        header->free_fn = vector_init_params.free_fn;
        header->calculate_optimal_capacity_fn = vector_init_params.calculate_optimal_capacity_fn;
        return header->data;
    }

    size_t Vector_get_element_size(void *vec_ptr) {
        return __vector_get_header(vec_ptr)->element_size;
    }

    size_t Vector_get_length(void *vec_ptr) {
        return __vector_get_header(vec_ptr)->length;
    }

    size_t Vector_get_capacity(void *vec_ptr) {
        return __vector_get_header(vec_ptr)->capacity;
    }

    size_t Vector_get_initial_capacity(void *vec_ptr) {
        return __vector_get_header(vec_ptr)->initial_capacity;
    }

    bool Vector_is_full(void *vec_ptr) {
        __Vector_Header *header = __vector_get_header(vec_ptr);
        return header->length == header->capacity;
    }

    bool Vector_is_underfilled(void *vec_ptr) {
        __Vector_Header *header = __vector_get_header(vec_ptr);
        return header->capacity > header->initial_capacity && header->length * 2 < header->capacity;
    }

    bool Vector_is_empty(void *vec_ptr) {
        return Vector_get_length(vec_ptr) == 0;
    }

    void Vector_set_initial_capacity(void *vec_ptr, size_t initial_capacity) {
        // This function calls __vector_resize_if_needed to immidiately resize the vector if it needs to (the default optimal capacity calculation relies on the initial capacity)
        // This is a fine approach because if the user is sane, they will call this function only once in the lifetime of the vector if not call it at all
        // which I find it better than overcomplicating the API and making Vector_init take an initial capacity argument.
        // However, i might consider using optional arguments trick in the future to make Vector_init take optional arguments, but for now, this is fine. (hopefully i don't do that without forgetting to change this comment :D)
        __vector_get_header(vec_ptr)->initial_capacity = initial_capacity;
        __vector_resize_if_needed(vec_ptr);
    }

    void Vector_set_free_fn(void *vec_ptr, Vector_free_fn free_fn) {
        __vector_get_header(vec_ptr)->free_fn = free_fn;
    }

    void Vector_set_calculate_optimal_capacity_fn(void *vec_ptr, Vector_calculate_optimal_capacity_fn calculate_optimal_capacity_fn) {
        __vector_get_header(vec_ptr)->calculate_optimal_capacity_fn = calculate_optimal_capacity_fn;
    }

#endif // VECTOR_IMPLEMENTATION

#if LANGUAGE_CPP // C++ support
}
#endif // C++ support

#endif // VECTOR_H
