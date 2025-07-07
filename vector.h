#ifndef VECTOR_H
#define VECTOR_H

#include "./modules/system_env/system_env.h"

#if LANGUAGE_CPP // C++ support
extern "C" {    // prevent name mangling
#endif         // C++ support

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


#define VECTOR_DEFAULT_INITIAL_CAPACITY 4

typedef void (*Vector_free_fn)(void *vec_ptr);
typedef size_t (*Vector_calculate_optimal_capacity_fn)(void *vec_ptr);

// i am storing the element size in the header so that i can have a workaround
// for some functions for the compilers that do not support 'typeof' keyword
typedef struct __Vector_Header {
    size_t element_size;
    size_t length;
    size_t capacity;
    size_t initial_capacity;
    Vector_free_fn free_fn; // Cast the pointer to the vector to the type you want and free it
    Vector_calculate_optimal_capacity_fn calculate_optimal_capacity_fn;
    char data[];
} __Vector_Header;


#if !COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
// Pass this as a last argument if your compiler does not support STATEMENT_EXPRESSIONS and you don't care about the result
// Note: you have to cast this value to a pointer type other than void (preferrably the same type as the vector)
// example:
// ```
//     int *vec = Vector_init(int);
//     Vector_push(&vec, 10);
//     Vector_pop(&vec, (int *)VECTOR_IGNORE_RETURN)
// ```
void *VECTOR_IGNORE_RETURN = NULL;
#endif // !COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

/**
 * Internal
 * 
 * Returns the header of a vector
 * @param vec_ptr [T**]              - A reference to the vector
 * @return        [__Vector_Header*] - The header of the vector
 * @throw         [assert]           - If the reference to the vector is NULL
 * @throw         [assert]           - If the vector is NULL
 */
__Vector_Header *__vector_get_header(void *vec_ptr);

/**
 * Internal
 * 
 * Resizes the vector if the capacity is not optimal
 * @param vec_ptr [T**]    - A reference to the vector
 * @throw         [assert] - If the reference to the vector is NULL
 * @throw         [assert] - If the vector is NULL
 */
void __vector_resize_if_needed(void *vec_ptr);

/**
 * Internal
 * 
 * Initializes a vector
 * @param element_size [size_t] - The size of the vector type
 * @return             [T*]     - The array of data
 * @throw              [assert] - If malloc fails
 */
void *__vector_init(size_t element_size);

/**
 * Public
 * 
 * Initializes a vector
 * @param __T__ [type] - The type of the vector
 * @return      [T*]   - The vector
 */
#define Vector_init(__T__) (__T__*)__vector_init(sizeof(__T__))


/**
 * Public
 * 
 * Gets the size of an element in the vector
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [size_t] - The element size of the vector
 * @throw         [assert] - If the reference to the vector is NULL
 * @throw         [assert] - If the vector is NULL
 */
size_t Vector_get_element_size(void *vec_ptr);

/**
 * Public
 * 
 * Returns the number of elements in a vector
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [size_t] - The number of elements in the vector
 * @throw         [assert] - If the reference to the vector is NULL
 * @throw         [assert] - If the vector is NULL
 */
size_t Vector_get_length(void *vec_ptr);

/**
 * Public
 * 
 * Returns the capacity of a vector
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [size_t] - The capacity of the vector
 * @throw         [assert] - If the reference to the vector is NULL
 * @throw         [assert] - If the vector is NULL
 */
size_t Vector_get_capacity(void *vec_ptr);

/**
 * Public
 * 
 * Returns the initial capacity of a vector
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [size_t] - The initial capacity of the vector
 * @throw         [assert] - If the reference to the vector is NULL
 * @throw         [assert] - If the vector is NULL
 */
size_t Vector_get_initial_capacity(void *vec_ptr);

/**
 * Public
 * 
 * Checks if the vector is full
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [bool]   - True if the vector is full, false otherwise
 * @throw         [assert] - If the reference to the vector is NULL
 * @throw         [assert] - If the vector is NULL
 */
bool Vector_is_full(void *vec_ptr);

/**
 * Public
 * 
 * Returns true if the capacity is less than half full (there is a lot of unused space) and capacity > initial_capacity (the initial_capacity is the minimum capacity)
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [bool]   - True if the vector is underfilled, false otherwise
 * @throw         [assert] - If the reference to the vector is NULL
 * @throw         [assert] - If the vector is NULL
 */
bool Vector_is_underfilled(void *vec_ptr);

/**
 * Public
 * 
 * Checks if the vector is empty
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [bool]   - True if the vector is empty, false otherwise
 * @throw         [assert] - If the reference to the vector is NULL
 * @throw         [assert] - If the vector is NULL
 */
bool Vector_is_empty(void *vec_ptr);

/**
 * Public
 * 
 * Sets the custom free function of the vector
 * @param vec_ptr [T**]            - a reference to the vector
 * @param free_fn [Vector_free_fn] - the free function to free the vector
 * @throw         [assert]         - if the reference to the vector is NULL
 * @throw         [assert]         - if the vector is NULL
 */
void Vector_set_free_fn(void *vec_ptr, Vector_free_fn free_fn);

/**
 * Public
 * 
 * Sets the custom initial capacity of the vector and resizes it immediately
 * The default initial capacity is 4
 * @param vec_ptr          [T**]    - A reference to the vector
 * @param initial_capacity [size_t] - The initial capacity of the vector
 * @throw                  [assert] - If the reference to the vector is NULL
 * @throw                  [assert] - If the vector is NULL
 */
void Vector_set_initial_capacity(void *vec_ptr, size_t initial_capacity);

/**
 * Public
 * 
 * Sets the custom function that calculates the optimal capacity of the vector
 * @param vec_ptr                       [T**]                                  - A reference to the vector
 * @param calculate_optimal_capacity_fn [Vector_calculate_optimal_capacity_fn] - The function that calculates the optimal capacity of the vector
 * @throw                               [assert]                               - If the reference to the vector is NULL
 * @throw                               [assert]                               - If the vector is NULL
 */
void Vector_set_calculate_optimal_capacity_fn(void *vec_ptr, Vector_calculate_optimal_capacity_fn calculate_optimal_capacity_fn);

/**
 * Public
 * 
 * Destroys and frees the vector
 * @param __vec_ptr__ [T**]    - A reference to the vector
 * @throw             [assert] - If the reference to the vector is NULL
 * @throw             [assert] - If the vector is NULL
 */
#define Vector_destroy(__vec_ptr__) do { \
    assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
    __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
    if (__header__->free_fn == NULL) { \
        free(__header__); \
        (*(__vec_ptr__)) = NULL; \
    } else { \
        __header__->free_fn((void*)(__vec_ptr__)); \
    } \
} while (0)

#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Gets the index of the value in the vector
     * @param __vec_ptr__            [T**]           - A reference to the vector
     * @param __value__              [size_t]        - The value to get the index of
     * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values (should return true if the values are equal, false otherwise), the first argument is the value in the vector, the second argument is the value given as an argument
     * @return                       [size_t]        - The index of the value in the vector
     * @throw                        [assert]        - If the reference to the vector is NULL
     * @throw                        [assert]        - If the vector is NULL
     * @throw                        [assert]        - If the value does not exist in the vector
     */
    #define Vector_index(__vec_ptr__, __value__, __boolean_comparator__) ({ \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        bool __found__ = false; \
        size_t __i__ = 0; \
        for ( ; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
            if ((__boolean_comparator__)((*(__vec_ptr__))[__i__], (__value__))) { \
                __found__ = true; \
                break; \
            } \
        } \
        assert(__found__); \
        __i__; \
    })
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Gets the index of the value in the vector
     * @param __vec_ptr__            [T**]           - A reference to the vector
     * @param __value__              [size_t]        - The value to get the index of
     * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values (should return true if the values are equal, false otherwise), the first argument is the value in the vector, the second argument is the value given as an argument
     * @param __result_ptr__         [size_t*]       - A pointer to the variable to store the index in, if NULL, the result will not be stored but the function will execute normally
     * @throw                        [assert]        - If the reference to the vector is NULL
     * @throw                        [assert]        - If the vector is NULL
     * @throw                        [assert]        - If the value does not exist in the vector
     */
    #define Vector_index(__vec_ptr__, __value__, __boolean_comparator__, __result_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        bool __found__ = false; \
        size_t __i__ = 0; \
        for ( ; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
            if ((__boolean_comparator__)((*(__vec_ptr__))[__i__], (__value__))) { \
                __found__ = true; \
                break; \
            } \
        } \
        assert(__found__); \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __i__; } \
    } while (0)
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS


#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Gets the count of the value in the vector
     * @param __vec_ptr__            [T**]           - A reference to the vector
     * @param __value__              [T]             - The value to get the count of
     * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value given as an argument
     * @return                       [size_t]        - The count of the value in the vector
     * @throw                        [assert]        - If the reference to the vector is NULL
     * @throw                        [assert]        - If the vector is NULL
     */
    #define Vector_count(__vec_ptr__, __value__, __boolean_comparator__) ({ \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        size_t __count__ = 0; \
        for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
            if ((__boolean_comparator__)((*(__vec_ptr__))[__i__], (__value__))) { \
                __count__++; \
            } \
        } \
        __count__; \
    })
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Gets the count of the value in the vector
     * @param __vec_ptr__            [T**]           - A reference to the vector
     * @param __value__              [T]             - The value to get the count of
     * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value given as an argument
     * @param __result_ptr__         [size_t*]       - A pointer to the variable to store the count in, if NULL, the result will not be stored but the function will execute normally
     * @throw                        [assert]        - If the reference to the vector is NULL
     * @throw                        [assert]        - If the vector is NULL
     */
    #define Vector_count(__vec_ptr__, __value__, __boolean_comparator__, __result_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        size_t __count__ = 0; \
        for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
            if ((__boolean_comparator__)((*(__vec_ptr__))[__i__], (__value__))) { \
                __count__++; \
            } \
        } \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __count__; } \
    } while (0)
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

/**
 * Public
 * 
 * Pushes a value to the end of the vector
 * @param __vec_ptr__ [T**]    - A reference to the vector
 * @param __value__   [T]      - The value to push to the vector
 * @throw             [assert] - If the reference to the vector is NULL
 * @throw             [assert] - If the vector is NULL
 * @throw             [assert] - If malloc fails
 */
#define Vector_push(__vec_ptr__, __value__) do { \
    assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
    __vector_resize_if_needed((__vec_ptr__)); \
    __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
    (*(__vec_ptr__))[__header__->length++] = (__value__); \
} while (0)

/**
 * Public
 * 
 * Inserts a value at the specified index in the vector
 * @param __vec_ptr__ [T**]    - A reference to the vector
 * @param __index__   [size_t] - The index to insert the value at
 * @param __value__   [T]      - The value to insert
 * @throw             [assert] - If the reference to the vector is NULL
 * @throw             [assert] - If the vector is NULL
 * @throw             [assert] - If the index is out of bounds
 * @throw             [assert] - If malloc fails
 */
#define Vector_insert_at(__vec_ptr__, __index__, __value__) do { \
    assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
    __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
    assert((__index__) >= 0 && (__index__) <= __header__->length); \
    __vector_resize_if_needed((__vec_ptr__)); \
    memmove((*(__vec_ptr__)) + (__index__) + 1, (*(__vec_ptr__)) + (__index__), (__header__->length - (__index__)) * __header__->element_size); \
    (*(__vec_ptr__))[(__index__)] = (__value__); \
    __header__->length++; \
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
     * @throw                         [assert]        - If the reference to the vector is NULL
     * @throw                         [assert]        - If the vector is NULL
     * @throw                         [assert]        - If malloc fails
     */
    #define Vector_insert_sorted(__vec_ptr__, __value__, __ordering_comparator__) ({ \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __vector_resize_if_needed((__vec_ptr__)); \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
        size_t __low__ = 0; \
        size_t __high__ = __header__->length; \
        while (__low__ < __high__) { \
            size_t __mid__ = (__low__ + __high__) / 2; \
            if ((__ordering_comparator__)((*(__vec_ptr__))[__mid__], (__value__)) >= 0) { \
                __high__ = __mid__; \
            } else { \
                __low__ = __mid__ + 1; \
            } \
        } \
        Vector_insert_at((__vec_ptr__), __low__, (__value__)); \
        __low__; \
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
     * @throw                         [assert]        - If the reference to the vector is NULL
     * @throw                         [assert]        - If the vector is NULL
     * @throw                         [assert]        - If malloc fails
     */
    #define Vector_insert_sorted(__vec_ptr__, __value__, __ordering_comparator__, __result_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __vector_resize_if_needed((__vec_ptr__)); \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
        size_t __low__ = 0; \
        size_t __high__ = __header__->length; \
        while (__low__ < __high__) { \
            size_t __mid__ = (__low__ + __high__) / 2; \
            if ((__ordering_comparator__)((*(__vec_ptr__))[__mid__], (__value__)) >= 0) { \
                __high__ = __mid__; \
            } else { \
                __low__ = __mid__ + 1; \
            } \
        } \
        Vector_insert_at((__vec_ptr__), __low__, (__value__)); \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __low__; } \
    } while (0)
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

/**
 * Public
 * 
 * Appends a copy of the values in the second vector to the first vector
 * @param __vec_ptr1__ [T**]    - A reference to the first vector
 * @param __vec_ptr2__ [T**]    - A reference to the second vector
 * @throw              [assert] - If the reference to the first vector is NULL
 * @throw              [assert] - If the first vector is NULL
 * @throw              [assert] - If the reference to the second vector is NULL
 * @throw              [assert] - If the second vector is NULL
 * @throw              [assert] - If malloc fails
 */
#define Vector_concat(__vec_ptr1__, __vec_ptr2__) do { \
    assert(((__vec_ptr1__) != NULL) && ((*(__vec_ptr1__)) != NULL)); \
    assert(((__vec_ptr2__) != NULL) && ((*(__vec_ptr2__)) != NULL)); \
    for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr2__)); __i__++) { \
        Vector_push((__vec_ptr1__), (*(__vec_ptr2__))[__i__]); \
    } \
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
        #define Vector_pop(__vec_ptr__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
            assert(__header__->length > 0); \
            typeof(**(__vec_ptr__)) __value__ = (*(__vec_ptr__))[__header__->length - 1]; \
            __header__->length--; \
            __vector_resize_if_needed((__vec_ptr__)); \
            __value__; \
        })
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Pops the last value from the vector and returns it
         * @param __vec_ptr__          [T**]    - A reference to the vector
         * @param __vec_element_type__ [type]   - The type of the vector
         * @return                     [T]      - The value popped from the vector
         * @throw                      [assert] - If the vector is NULL
         * @throw                      [assert] - If malloc fails
         * @throw                      [assert] - If the vector is empty
         */
        #define Vector_pop(__vec_ptr__, __vec_element_type__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
            assert(__header__->length > 0); \
            __vec_element_type__ __value__ = (*(__vec_ptr__))[__header__->length - 1]; \
            __header__->length--; \
            __vector_resize_if_needed((__vec_ptr__)); \
            __value__; \
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
    #define Vector_pop(__vec_ptr__, __result_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
        assert(__header__->length > 0); \
        if ((__result_ptr__) != NULL) { \
            (*(__result_ptr__)) = (*(__vec_ptr__))[__header__->length - 1]; \
        } \
        __header__->length--; \
        __vector_resize_if_needed((__vec_ptr__)); \
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
        #define Vector_remove_at(__vec_ptr__, __index__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
            assert((__index__) >= 0 && (__index__) < __header__->length); \
            typeof(**(__vec_ptr__)) __value__ = (*(__vec_ptr__))[(__index__)]; \
            memmove((*(__vec_ptr__)) + (__index__), (*(__vec_ptr__)) + (__index__) + 1, (__header__->length - (__index__) - 1) * __header__->element_size); \
            __header__->length--; \
            __vector_resize_if_needed((__vec_ptr__)); \
            __value__; \
        })
    #else // COMPILER_SUPPORTS_TYPEOF
        /**
         * Public
         * 
         * Removes the value at the specified index from the vector
         * @param __vec_ptr__          [T**]    - A reference to the vector
         * @param __index__            [size_t] - The index to remove the value from
         * @param __vec_element_type__ [type]   - The type of the vector
         * @return                     [T]      - The value removed from the vector
         * @throw                      [assert] - If the vector is NULL
         * @throw                      [assert] - If malloc fails
         * @throw                      [assert] - If the index is out of bounds
         */
        #define Vector_remove_at(__vec_ptr__, __index__, __vec_element_type__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
            assert((__index__) >= 0 && (__index__) < __header__->length); \
            __vec_element_type__ __value__ = (*(__vec_ptr__))[(__index__)]; \
            memmove((*(__vec_ptr__)) + (__index__), (*(__vec_ptr__)) + (__index__) + 1, (__header__->length - (__index__) - 1) * __header__->element_size); \
            __header__->length--; \
            __vector_resize_if_needed((__vec_ptr__)); \
            __value__; \
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
    #define Vector_remove_at(__vec_ptr__, __index__, __result_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
        assert((__index__) >= 0 && (__index__) < __header__->length); \
        if ((__result_ptr__) != NULL) { \
            (*(__result_ptr__)) = (*(__vec_ptr__))[(__index__)]; \
        } \
        memmove((*(__vec_ptr__)) + (__index__), (*(__vec_ptr__)) + (__index__) + 1, (__header__->length - (__index__) - 1) * __header__->element_size); \
        __header__->length--; \
        __vector_resize_if_needed((__vec_ptr__)); \
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
         * @throw                        [assert]        - If the reference to the vector is NULL
         * @throw                        [assert]        - If the vector is NULL
         * @throw                        [assert]        - If malloc fails
         * @throw                        [assert]        - If the value does not exist in the vector
         */
        #define Vector_remove_value(__vec_ptr__, __value__, __boolean_comparator__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            size_t __index__ = Vector_index((__vec_ptr__), (__value__), (__boolean_comparator__)); \
            Vector_remove_at((__vec_ptr__), __index__); \
            __index__; \
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
         * @throw                        [assert]        - If the reference to the vector is NULL
         * @throw                        [assert]        - If the vector is NULL
         * @throw                        [assert]        - If malloc fails
         * @throw                        [assert]        - If the value does not exist in the vector
         */
#define Vector_remove_value(__vec_ptr__, __value__, __boolean_comparator__, __vec_element_type__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            size_t __index__ = Vector_index((__vec_ptr__), (__value__), (__boolean_comparator__)); \
            Vector_remove_at((__vec_ptr__), __index__, __vec_element_type__); \
            __index__; \
        })
    #endif // COMPILER_SUPPORTS_TYPEOF
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Removes the first occurrence of the value from the vector
     * @param __vec_ptr__            [T**]           - A reference to the vector
     * @param __value__              [T]             - The value to remove
     * @param __boolean_comparator__ [int (*)(T, T)] - The boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value to search for
     * @param __result_ptr__         [size_t*]       - A pointer to the variable to store the index of the value removed from the vector, if NULL, the result will not be stored but the function will execute normally
     * @throw                        [assert]        - If the reference to the vector is NULL
     * @throw                        [assert]        - If the vector is NULL
     * @throw                        [assert]        - If malloc fails
     * @throw                        [assert]        - If the value does not exist in the vector
     */
    #define Vector_remove_value(__vec_ptr__, __value__, __boolean_comparator__, __result_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        size_t __index__; \
        Vector_index((__vec_ptr__), (__value__), (__boolean_comparator__), &__index__); \
        Vector_remove_at((__vec_ptr__), __index__, (char *)VECTOR_IGNORE_RETURN); \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __index__; } \
    } while (0)
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS


/**
 * Public
 * 
 * Clears all the elements in the vector
 * @param __vec_ptr__ [T**]    - A reference to the vector
 * @throw             [assert] - If the reference to the vector is NULL
 * @throw             [assert] - If the vector is NULL
 */
#define Vector_clear(__vec_ptr__) do { \
    assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
    __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
    __header__->length = 0; \
    __vector_resize_if_needed((__vec_ptr__)); \
} while (0)


// the result is casted to void* to avoid the casting warning
#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Returns a shallow copy of the vector
     * @param __vec_ptr__ [T**]    - A reference to the vector
     * @return            [T*]     - The copied vector
     * @throw             [assert] - If the reference to the vector is NULL
     * @throw             [assert] - If the vector is NULL
     * @throw             [assert] - If malloc fails
     * @note the returned result is a shallow copy, if the vector contains pointers to objects, the objects will not be copied
     */
    #define Vector_copy(__vec_ptr__) ({ \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __Vector_Header *__old_vec__ = __vector_get_header((__vec_ptr__)); \
        __Vector_Header *__new_vec__ = (__Vector_Header *)malloc(sizeof(__Vector_Header) + __old_vec__->capacity * __old_vec__->element_size); \
        assert(__new_vec__ != NULL); \
        memcpy(__new_vec__, __old_vec__, sizeof(__Vector_Header) + __old_vec__->length * __old_vec__->element_size); \
        (void*)__new_vec__->data; \
    })
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Returns a shallow copy of the vector
     * @param __old_vec_ptr__ [T**]    - A reference to the vector
     * @param __new_vec_ptr__ [T**]    - A reference to the copied vector
     * @throw                 [assert] - If the reference to the vector is NULL
     * @throw                 [assert] - If the vector is NULL
     * @throw                 [assert] - If malloc fails
     * @note the returned result is a shallow copy, if the vector contains pointers to objects, the objects will not be copied
     */
    #define Vector_copy(__old_vec_ptr__, __new_vec_ptr__) do { \
        assert(((__old_vec_ptr__) != NULL) && ((*(__old_vec_ptr__)) != NULL)); \
        assert((__new_vec_ptr__) != NULL); \
        __Vector_Header *__old_vec__ = __vector_get_header((__old_vec_ptr__)); \
        __Vector_Header *__new_vec__ = (__Vector_Header *)malloc(sizeof(__Vector_Header) + __old_vec__->capacity * __old_vec__->element_size); \
        assert(__new_vec__ != NULL); \
        memcpy(__new_vec__, __old_vec__, sizeof(__Vector_Header) + __old_vec__->length * __old_vec__->element_size); \
        (*(__new_vec_ptr__)) = (void*)__new_vec__->data; \
    } while (0)
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

#if COMPILER_SUPPORTS_TYPEOF
    /**
     * Public
     * 
     * Reverses the vector in place
     * @param __vec_ptr__ [T**]    - A reference to the vector
     * @throw             [assert] - If the reference to the vector is NULL
     * @throw             [assert] - If the vector is NULL
     */
    #define Vector_reverse(__vec_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
        typeof(**(__vec_ptr__)) __temp__; \
        for (size_t __i__ = 0; __i__ < __header__->length / 2; __i__++) { \
            __temp__ = (*(__vec_ptr__))[__i__]; \
            (*(__vec_ptr__))[__i__] = (*(__vec_ptr__))[__header__->length - __i__ - 1]; \
            (*(__vec_ptr__))[__header__->length - __i__ - 1] = __temp__; \
        } \
    } while (0)
#else // COMPILER_SUPPORTS_TYPEOF
    /**
     * Public
     * 
     * Reverses the vector in place
     * @param __vec_ptr__          [T**]    - A reference to the vector
     * @param __vec_element_type__ [type]   - A reference to the vector
     * @throw                      [assert] - If the reference to the vector is NULL
     * @throw                      [assert] - If the vector is NULL
     */
    #define Vector_reverse(__vec_ptr__, __vec_element_type__) do {                             \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
        __vec_element_type__ __temp__; \
        for (size_t __i__ = 0; __i__ < __header__->length / 2; __i__++) { \
            __temp__ = (*(__vec_ptr__))[__i__]; \
            (*(__vec_ptr__))[__i__] = (*(__vec_ptr__))[__header__->length - __i__ - 1]; \
            (*(__vec_ptr__))[__header__->length - __i__ - 1] = __temp__; \
        } \
    } while (0)
#endif // COMPILER_SUPPORTS_TYPEOF

#if COMPILER_SUPPORTS_TYPEOF
    #define __merge__(__arr__, __left_size__, __mid__, __right_size__, __ordering_comparator__) do { \
        int __i__, __j__, __k__; \
        int __n1__ = (__mid__) - (__left_size__) + 1; \
        int __n2__ =  (__right_size__) - (__mid__); \
        typeof(*(__arr__)) *__left_array__ = (typeof(__arr__))malloc(__n1__ * sizeof(*(__arr__))); \
        typeof(*(__arr__)) *__right_array__ = (typeof(__arr__))malloc(__n2__ * sizeof(*(__arr__))); \
        for (__i__ = 0; __i__ < __n1__; __i__++) { \
            __left_array__[__i__] = (__arr__)[(__left_size__) + __i__]; \
        } \
        for (__j__ = 0; __j__ < __n2__; __j__++) { \
            __right_array__[__j__] = (__arr__)[(__mid__) + 1 + __j__]; \
        } \
        __i__ = 0; __j__ = 0; __k__ = (__left_size__); \
        while (__i__ < __n1__ && __j__ < __n2__) { \
            if ((__ordering_comparator__)(__left_array__[__i__], __right_array__[__j__]) <= 0) { \
                (__arr__)[__k__++] = __left_array__[__i__++]; \
            } else { \
                (__arr__)[__k__++] = __right_array__[__j__++]; \
            } \
        } \
        while (__i__ < __n1__) { \
            (__arr__)[__k__++] = __left_array__[__i__++]; \
        } \
        while (__j__ < __n2__) { \
            (__arr__)[__k__++] = __right_array__[__j__++]; \
        } \
        free(__left_array__); \
        free(__right_array__); \
    } while(0)

    #define __merge_sort__(__arr__, __n__, __ordering_comparator__) do { \
        int __curr_size__, __left_start__; \
        for (__curr_size__ = 1; __curr_size__ <= (__n__) - 1; __curr_size__ = 2 * __curr_size__) { \
            for (__left_start__ = 0; __left_start__ < (__n__) - 1; __left_start__ += 2 * __curr_size__) { \
                int __mid__ = __left_start__ + __curr_size__ < (__n__) ? __left_start__ + __curr_size__ - 1 : (__n__) - 1; \
                int __right_end__ = __left_start__ + 2 * __curr_size__ < (__n__) ? __left_start__ + 2 * __curr_size__ - 1 : (__n__) - 1; \
                __merge__((__arr__), __left_start__, __mid__, __right_end__, (__ordering_comparator__)); \
            } \
        } \
    } while(0)

    /**
     * Public
     * 
     * Sorts the vector in place
     * @param __vec_ptr__             [T**]           - A reference to the vector
     * @param __ordering_comparator__ [int (*)(T, T)] - The ordering comparator function to compare the values, should return a positive number if the first value is greater than the second value, a negative number if the first value is less than the second value, and 0 if the values are equal
     * @throw                         [assert]        - If the reference to the vector is NULL
     * @throw                         [assert]        - If the vector is NULL
     */
    #define Vector_sort(__vec_ptr__, __ordering_comparator__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
        __merge_sort__((*(__vec_ptr__)), __header__->length, (__ordering_comparator__)); \
    } while (0)
#else // COMPILER_SUPPORTS_TYPEOF
    #define __merge__(__arr__, __left_size__, __mid__, __right_size__, __ordering_comparator__, __vec_element_type__) do { \
        int __i__, __j__, __k__; \
        int __n1__ = (__mid__) - (__left_size__) + 1; \
        int __n2__ =  (__right_size__) - (__mid__); \
        __vec_element_type__ *__left_array__ = (__vec_element_type__ *)malloc(__n1__ * sizeof(*(__arr__))); \
        __vec_element_type__ *__right_array__ = (__vec_element_type__ *)malloc(__n2__ * sizeof(*(__arr__))); \
        for (__i__ = 0; __i__ < __n1__; __i__++) { \
            __left_array__[__i__] = (__arr__)[(__left_size__) + __i__]; \
        } \
        for (__j__ = 0; __j__ < __n2__; __j__++) { \
            __right_array__[__j__] = (__arr__)[(__mid__) + 1 + __j__]; \
        } \
        __i__ = 0; __j__ = 0; __k__ = (__left_size__); \
        while (__i__ < __n1__ && __j__ < __n2__) { \
            if ((__ordering_comparator__)(__left_array__[__i__], __right_array__[__j__]) <= 0) { \
                (__arr__)[__k__++] = __left_array__[__i__++]; \
            } else { \
                (__arr__)[__k__++] = __right_array__[__j__++]; \
            } \
        } \
        while (__i__ < __n1__) { \
            (__arr__)[__k__++] = __left_array__[__i__++]; \
        } \
        while (__j__ < __n2__) { \
            (__arr__)[__k__++] = __right_array__[__j__++]; \
        } \
        free(__left_array__); \
        free(__right_array__); \
    } while(0)

    #define __merge_sort__(__arr__, __n__, __ordering_comparator__, __vec_element_type__) do { \
        int __curr_size__, __left_start__; \
        for (__curr_size__ = 1; __curr_size__ <= (__n__) - 1; __curr_size__ = 2 * __curr_size__) { \
            for (__left_start__ = 0; __left_start__ < (__n__) - 1; __left_start__ += 2 * __curr_size__) { \
                int __mid__ = __left_start__ + __curr_size__ < (__n__) ? __left_start__ + __curr_size__ - 1 : (__n__) - 1; \
                int __right_end__ = __left_start__ + 2 * __curr_size__ < (__n__) ? __left_start__ + 2 * __curr_size__ - 1 : (__n__) - 1; \
                __merge__((__arr__), __left_start__, __mid__, __right_end__, (__ordering_comparator__), __vec_element_type__); \
            } \
        } \
    } while(0)

    /**
     * Public
     * 
     * Sorts the vector in place
     * @param __vec_ptr__             [T**]           - A reference to the vector
     * @param __ordering_comparator__ [int (*)(T, T)] - The ordering comparator function to compare the values, should return a positive number if the first value is greater than the second value, a negative number if the first value is less than the second value, and 0 if the values are equal
     * @param __vec_element_type__    [type]          - The type of the elements in the vector
     * @throw                         [assert]        - If the reference to the vector is NULL
     * @throw                         [assert]        - If the vector is NULL
     */
    #define Vector_sort(__vec_ptr__, __ordering_comparator__, __vec_element_type__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
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
         * @throw             [assert]      - If the reference to the vector is NULL
         * @throw             [assert]      - If the vector is NULL
         * @throw             [assert]      - If malloc fails
         */
        #define Vector_filter(__vec_ptr__, __filter__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            typeof(*(__vec_ptr__)) __new_vec__ = Vector_init(typeof(**(__vec_ptr__))); \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
                if ((__filter__)((*(__vec_ptr__))[__i__])) { \
                    Vector_push(&__new_vec__, (*(__vec_ptr__))[__i__]); \
                } \
            } \
            __new_vec__; \
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
         * @throw                      [assert]      - If the reference to the vector is NULL
         * @throw                      [assert]      - If the vector is NULL
         * @throw                      [assert]      - If malloc fails
         */
        #define Vector_filter(__vec_ptr__, __filter__, __vec_element_type__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            __vec_element_type__ *__new_vec__ = Vector_init(__vec_element_type__); \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
                if ((__filter__)((*(__vec_ptr__))[__i__])) { \
                    Vector_push(&__new_vec__, (*(__vec_ptr__))[__i__]); \
                } \
            } \
            __new_vec__; \
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
         * @throw                 [assert]      - If the reference to the vector is NULL
         * @throw                 [assert]      - If the vector is NULL
         * @throw                 [assert]      - If the reference to the new vector is NULL
         * @throw                 [assert]      - If malloc fails
         */
        #define Vector_filter(__vec_ptr__, __filter__, __new_vec_ptr__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            assert((__new_vec_ptr__) != NULL); \
            (*(__new_vec_ptr__)) = Vector_init(typeof(**(__vec_ptr__))); \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
                if ((__filter__)((*(__vec_ptr__))[__i__])) { \
                    Vector_push((__new_vec_ptr__), (*(__vec_ptr__))[__i__]); \
                } \
            } \
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
         * @throw                      [assert]      - If the reference to the vector is NULL
         * @throw                      [assert]      - If the vector is NULL
         * @throw                      [assert]      - If the reference to the new vector is NULL
         * @throw                      [assert]      - If malloc fails
         */
        #define Vector_filter(__vec_ptr__, __filter__, __new_vec_ptr__, __vec_element_type__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            assert((__new_vec_ptr__) != NULL); \
            (*(__new_vec_ptr__)) = Vector_init(__vec_element_type__); \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
                if ((__filter__)((*(__vec_ptr__))[__i__])) { \
                    Vector_push((__new_vec_ptr__), (*(__vec_ptr__))[__i__]); \
                } \
            } \
        } while (0)
    #endif // COMPILER_SUPPORTS_TYPEOF
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

/**
 * Public
 * 
 * Applies a function to each value in the vector
 * @param __vec_ptr__ [T**]          - A reference to the vector
 * @param __func__    [void (*)(T*)] - The function to apply to each value, it takes a pointer to the value
 * @throw             [assert]       - If the reference to the vector is NULL
 * @throw             [assert]       - If the vector is NULL
 */
#define Vector_foreach(__vec_ptr__, __func__) do { \
    assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
    for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
        (__func__)(&((*(__vec_ptr__))[__i__])); \
    } \
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
     * @throw                          [assert]   - If the reference to the vector is NULL
     * @throw                          [assert]   - If the vector is NULL
     * @throw                          [assert]   - If malloc fails
     */
    #define Vector_map(__vec_ptr__, __mapper__, __new_vec_element_type__) ({ \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __new_vec_element_type__ *__new_vec__ = Vector_init(__new_vec_element_type__); \
        for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
            Vector_push(&__new_vec__, (__mapper__)((*(__vec_ptr__))[__i__])); \
        } \
        __new_vec__; \
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
         * @throw                 [assert]   - If the reference to the vector is NULL
         * @throw                 [assert]   - If the vector is NULL
         * @throw                 [assert]   - If the reference to the new vector is NULL
         * @throw                 [assert]   - If malloc fails
         */
        #define Vector_map(__vec_ptr__, __mapper__, __new_vec_ptr__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            assert((__new_vec_ptr__) != NULL); \
            (*(__new_vec_ptr__)) = Vector_init(typeof(**(__new_vec_ptr__))); \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
                Vector_push((__new_vec_ptr__), (__mapper__)((*(__vec_ptr__))[__i__])); \
            } \
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
         * @throw                          [assert]   - If the reference to the vector is NULL
         * @throw                          [assert]   - If the vector is NULL
         * @throw                          [assert]   - If the reference to the new vector is NULL
         * @throw                          [assert]   - If malloc fails
         */
        #define Vector_map(__vec_ptr__, __mapper__, __new_vec_ptr__, __new_vec_element_type__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            assert((__new_vec_ptr__) != NULL); \
            (*(__new_vec_ptr__)) = Vector_init(__new_vec_element_type__); \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
                Vector_push((__new_vec_ptr__), (__mapper__)((*(__vec_ptr__))[__i__])); \
            } \
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
         * @throw                   [assert]      - If the reference to the vector is NULL
         * @throw                   [assert]      - If the vector is NULL
         */
        #define Vector_reduce(__vec_ptr__, __reducer__, __initial_value__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            typeof((__initial_value__)) accumulator = (__initial_value__); \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
                accumulator = (__reducer__)(accumulator, (*(__vec_ptr__))[__i__]); \
            } \
            accumulator; \
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
         * @throw                      [assert]      - If the reference to the vector is NULL
         * @throw                      [assert]      - If the vector is NULL
         */
        #define Vector_reduce(__vec_ptr__, __reducer__, __initial_value__, __accumulator_type__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            __accumulator_type__ accumulator = (__initial_value__); \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
                accumulator = (__reducer__)(accumulator, (*(__vec_ptr__))[__i__]); \
            } \
            accumulator; \
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
         * @throw                   [assert]      - If the reference to the vector is NULL
         * @throw                   [assert]      - If the vector is NULL
         */
        #define Vector_reduce(__vec_ptr__, __reducer__, __initial_value__, __result_ptr__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            typeof((__initial_value__)) __accumulator__ = (__initial_value__); \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
                __accumulator__ = (__reducer__)(__accumulator__, (*(__vec_ptr__))[__i__]); \
            } \
            if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __accumulator__; } \
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
         * @throw                      [assert]      - If the reference to the vector is NULL
         * @throw                      [assert]      - If the vector is NULL
         */
        #define Vector_reduce(__vec_ptr__, __reducer__, __initial_value__, __result_ptr__, __accumulator_type__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            __accumulator_type__ __accumulator__ = (__initial_value__); \
            for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
                __accumulator__ = (__reducer__)(__accumulator__, (*(__vec_ptr__))[__i__]); \
            } \
            if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __accumulator__; } \
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
     * @throw             [assert]      - If the reference to the vector is NULL
     * @throw             [assert]      - If the vector is NULL
     */
    #define Vector_any(__vec_ptr__, __func__) ({ \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        bool __any__ = false; \
        for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
            if ((__func__)((*(__vec_ptr__))[__i__])) { \
                __any__ = true; \
                break; \
            } \
        } \
        __any__; \
    })
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Checks if any value in the vector satisfies the function
     * @param __vec_ptr__    [T**]         - A reference to the vector
     * @param __func__       [bool (*)(T)] - The function to check if any value satisfies
     * @param __result_ptr__ [bool*]       - A reference to the variable to store the result in, if NULL, the result will not be stored but the function will execute normally
     * @throw                [assert]      - If the reference to the vector is NULL
     * @throw                [assert]      - If the vector is NULL
     */
    #define Vector_any(__vec_ptr__, __func__, __result_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        bool __any__ = false; \
        for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
            if ((__func__)((*(__vec_ptr__))[__i__])) { \
                __any__ = true; \
                break; \
            } \
        } \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __any__; } \
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
     * @throw             [assert]      - If the reference to the vector is NULL
     * @throw             [assert]      - If the vector is NULL
     */
    #define Vector_all(__vec_ptr__, __func__) ({ \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        bool __all__ = true; \
        for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
            if (!(__func__)((*(__vec_ptr__))[__i__])) { \
                __all__ = false; \
                break; \
            } \
        } \
        __all__; \
    })
#else // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * Public
     * 
     * Checks if all values in the vector satisfy the function
     * @param __vec_ptr__    [T**]         - A reference to the vector
     * @param __func__       [bool (*)(T)] - The function to check if all values satisfy
     * @param __result_ptr__ [bool*]       - A reference to the variable to store the result in, if NULL, the result will not be stored but the function will execute normally
     * @throw                [assert]      - If the reference to the vector is NULL
     * @throw                [assert]      - If the vector is NULL
     */
    #define Vector_all(__vec_ptr__, __func__, __result_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        bool __all__ = true; \
        for (size_t __i__ = 0; __i__ < Vector_get_length((__vec_ptr__)); __i__++) { \
            if (!(__func__)((*(__vec_ptr__))[__i__])) { \
                __all__ = false; \
                break; \
            } \
        } \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __all__; } \
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
         * @throw             [assert] - If the reference to the vector is NULL
         * @throw             [assert] - If the vector is NULL
         * @throw             [assert] - If the start index is out of bounds
         * @throw             [assert] - If the end index is out of bounds
         * @throw             [assert] - If the step is less than or equal to 0
         */
        #define Vector_slice(__vec_ptr__, __start__, __end__, __step__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            typeof(*(__vec_ptr__)) __new_vec__ = Vector_init(typeof(**(__vec_ptr__))); \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
            assert((__start__) >= 0 && (__start__) < __header__->length); \
            assert((__end__) >= 0 && (__end__) <= __header__->length); \
            assert((__step__) > 0); \
            for (size_t __i__ = (__start__); __i__ < (__end__); __i__ += (__step__)) { \
                Vector_push(&__new_vec__, (*(__vec_ptr__))[__i__]); \
            } \
            __new_vec__; \
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
         * @throw                      [assert]      - If the reference to the vector is NULL
         * @throw                      [assert]      - If the vector is NULL
         * @throw                      [assert]      - If the start index is out of bounds
         * @throw                      [assert]      - If the end index is out of bounds
         * @throw                      [assert]      - If the step is less than or equal to 0
         */
        #define Vector_slice(__vec_ptr__, __start__, __end__, __step__, __vec_element_type__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            __vec_element_type__ *__new_vec__ = Vector_init(__vec_element_type__); \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
            assert((__start__) >= 0 && (__start__) < __header__->length); \
            assert((__end__) >= 0 && (__end__) <= __header__->length); \
            assert((__step__) > 0); \
            for (size_t __i__ = (__start__); __i__ < (__end__); __i__ += (__step__)) { \
                Vector_push(&__new_vec__, (*(__vec_ptr__))[__i__]); \
            } \
            __new_vec__; \
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
         * @throw                 [assert]      - If the reference to the vector is NULL
         * @throw                 [assert]      - If the vector is NULL
         * @throw                 [assert]      - If the start index is out of bounds
         * @throw                 [assert]      - If the end index is out of bounds
         * @throw                 [assert]      - If the step is less than or equal to 0
         */
        #define Vector_slice(__vec_ptr__, __start__, __end__, __step__, __new_vec_ptr__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            assert((__new_vec_ptr__) != NULL); \
            (*(__new_vec_ptr__)) = Vector_init(typeof(**(__vec_ptr__))); \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
            assert((__start__) >= 0 && (__start__) < __header__->length); \
            assert((__end__) >= 0 && (__end__) <= __header__->length); \
            assert((__step__) > 0); \
            for (size_t __i__ = (__start__); __i__ < (__end__); __i__ += (__step__)) { \
                Vector_push((__new_vec_ptr__), (*(__vec_ptr__))[__i__]); \
            } \
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
         * @throw                      [assert]      - If the reference to the vector is NULL
         * @throw                      [assert]      - If the vector is NULL
         * @throw                      [assert]      - If the start index is out of bounds
         * @throw                      [assert]      - If the end index is out of bounds
         * @throw                      [assert]      - If the step is less than or equal to 0
         */
        #define Vector_slice(__vec_ptr__, __start__, __end__, __step__, __new_vec_ptr__, __vec_element_type__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            assert((__new_vec_ptr__) != NULL); \
            (*(__new_vec_ptr__)) = Vector_init(__vec_element_type__); \
            __Vector_Header *__header__ = __vector_get_header((__vec_ptr__)); \
            assert((__start__) >= 0 && (__start__) < __header__->length); \
            assert((__end__) >= 0 && (__end__) <= __header__->length); \
            assert((__step__) > 0); \
            for (size_t __i__ = (__start__); __i__ < (__end__); __i__ += (__step__)) { \
                Vector_push((__new_vec_ptr__), (*(__vec_ptr__))[__i__]); \
            } \
        } while (0)
    #endif // COMPILER_SUPPORTS_TYPEOF
#endif // COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS

#if LANGUAGE_CPP // C++ support
}
#endif // C++ support

#endif // VECTOR_H
