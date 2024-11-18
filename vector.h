#ifndef VECTOR_H
#define VECTOR_H

#ifdef __cplusplus // C++ support
extern "C" {
#endif // C++ support

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "./modules/system_env/system_env.h"

#define VECTOR_DEFAULT_INITIAL_CAPACITY 4

// i am storing the element size in the header so that i can have a workaround
// for some functions for the compilers that do not support 'typeof' keyword
typedef struct __Vector_Header {
    size_t element_size;
    size_t length;
    size_t capacity;
    size_t initial_capacity;
    void (*free_fn) (void *vec_ptr); // you can cast the pointer to the vector to the type you want and free it
    size_t (*optimal_capacity_fn) (void *vec_ptr);
    char data[];
} __Vector_Header;


/**
 * INTERNAL
 * 
 * returns the header of a vector
 * @param vec_ptr [T**] - a reference to the vector
 * @return [__Vector_Header*] - the header of the vector
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * 
 */
__Vector_Header *__get_vector_header(void *vec_ptr);

/**
 * INTERNAL
 * 
 * resizes the vector if the capacity is not optimal
 * @param vec_ptr [T**] - a reference to the vector
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 */
void __vector_resize(void *vec_ptr);

/**
 * INTERNAL
 * 
 * initializes a vector
 * @param element_size [size_t] - the size of the vector type
 * @return [T*] - the array of data
 * @throw [assert] - if malloc fails
 */
void *__vector_init(size_t element_size);

/**
 * PUBLIC
 * 
 * initializes a vector
 * @param __T__ [type] - the type of the vector
 * @return [T*] - the vector
 * @example
 * ```
 * int *vec = Vector_init(int);
 * ```
 */
#define Vector_init(__T__) (__T__*)__vector_init(sizeof(__T__))


/**
 * PUBLIC
 * 
 * gets the size of an element in the vector
 * @param vec_ptr [T**] - a reference to the vector
 * @return [size_t] - the element size of the vector
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * size_t element_size = Vector_element_size(&vec); // should return sizeof(int)
 * ```
 */
size_t Vector_element_size(void *vec_ptr);

/**
 * PUBLIC
 * 
 * returns the number of elements in a vector
 * @param vec_ptr [T**] - a reference to the vector
 * @return [size_t] - the number of elements in the vector
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * size_t length = Vector_length(&vec); // should return 0
 * ```
 */
size_t Vector_length(void *vec_ptr);

/**
 * PUBLIC
 * 
 * returns the capacity of a vector
 * @param vec_ptr [T**] - a reference to the vector
 * @return [size_t] - the capacity of the vector
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * size_t capacity = Vector_capacity(&vec);
 * ```
 */
size_t Vector_capacity(void *vec_ptr);

/**
 * PUBLIC
 * 
 * returns the initial capacity of a vector
 * @param vec_ptr [T**] - a reference to the vector
 * @return [size_t] - the initial capacity of the vector
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * size_t initial_capacity = Vector_initial_capacity(&vec);
 * ```
 */
size_t Vector_initial_capacity(void *vec_ptr);

/**
 * PUBLIC
 * 
 * returns true if the capacity is all used up
 * @param vec_ptr [T**] - a reference to the vector
 * @return [bool] - true if the vector is full, false otherwise
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * bool is_full = Vector_is_full(&vec); // should return false
 */
bool Vector_is_full(void *vec_ptr);

/**
 * PUBLIC
 * 
 * returns true if the capacity is less than half full (there is a lot of unused space) and capacity > initial_capacity (the initial_capacity is the minimum capacity)
 * @param vec_ptr [T**] - a reference to the vector
 * @return [bool] - true if the vector is underfilled, false otherwise
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * bool is_underfilled = Vector_is_underfilled(&vec); // should return false
 * ```
 */
bool Vector_is_underfilled(void *vec_ptr);

/**
 * PUBLIC
 * 
 * returns true if the vector is empty
 * @param vec_ptr [T**] - a reference to the vector
 * @return [bool] - true if the vector is empty, false otherwise
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * bool is_empty = Vector_is_empty(&vec); // should return true
 * ```
 */
bool Vector_is_empty(void *vec_ptr);

/**
 * PUBLIC
 * 
 * sets the free function of the vector
 * @param vec_ptr [T**] - a reference to the vector
 * @param free_fn [void (*)(T*)] - the free function to free the vector
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * Vector_set_free_fn(&vec, void (void *vec_ptr) => {
 *      printf("freeing the vector\n");
 *      free(__get_vector_header(vec_ptr));
 * });
 * ```
 */
void Vector_set_free_fn(void *vec_ptr, void (*free_fn)(void *vec_ptr));

/**
 * PUBLIC
 * 
 * sets the initial capacity of the vector and resizes it immediately
 * the default initial capacity is 4
 * @param vec_ptr [T**] - a reference to the vector
 * @param initial_capacity [size_t] - the initial capacity of the vector
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * Vector_set_initial_capacity(&vec, 10);
 * ```
 */
void Vector_set_initial_capacity(void *vec_ptr, size_t initial_capacity);

/**
 * PUBLIC
 * 
 * sets the function that calculates the optimal capacity of the vector
 * @param vec_ptr [T**] - a reference to the vector
 * @param optimal_capacity_fn [size_t (*)(void*)] - the function that calculates the optimal capacity of the vector
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * Vector_set_optimal_capacity_fn(&vec, costum_get_optimal_capacity);
 * ```
 */
void Vector_set_optimal_capacity_fn(void *vec_ptr, size_t (*optimal_capacity_fn)(void *vec_ptr));

/**
 * PUBLIC
 * 
 * destroys the vector
 * @param __vec_ptr__ [T**] - a reference to the vector
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * Vector_destroy(&vec);
 * ```
 */
#define Vector_destroy(__vec_ptr__) do { \
    assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
    __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
    if (__header__->free_fn == NULL) { \
        free(__header__); \
        (*(__vec_ptr__)) = NULL; \
    } else { \
        __header__->free_fn((void*)(__vec_ptr__)); \
    } \
} while (0)

/**
 * PUBLIC
 * 
 * gets the index of the value in the vector
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @throw [assert] - if the value does not exist in the vector
 * @example
 * ```
 * int *vec = Vector_init(int);
 * size_t index = Vector_index(&vec, 5, boolean_comparator); // should throw an assert because the value is not found
 * ```
 */
#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * gets the index of the value in the vector
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @param __value__ [size_t] - the value to get the index of
     * @param __boolean_comparator__ [int (*)(T, T)] - the boolean comparator function to compare the values (should return true if the values are equal, false otherwise), the first argument is the value in the vector, the second argument is the value given as an argument
     * @return [size_t] - the index of the value in the vector
     */
    #define Vector_index(__vec_ptr__, __value__, __boolean_comparator__) ({ \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        bool __found__ = false; \
        size_t __i__ = 0; \
        for ( ; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
            if ((__boolean_comparator__)((*(__vec_ptr__))[__i__], (__value__))) { \
                __found__ = true; \
                break; \
            } \
        } \
        assert(__found__); \
        __i__; \
    })
#else
    /**
     * gets the index of the value in the vector
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @param __value__ [size_t] - the value to get the index of
     * @param __boolean_comparator__ [int (*)(T, T)] - the boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value given as an argument
     * @param __result_ptr__ [size_t*] - a pointer to the variable to store the index in, if NULL, the result will not be stored but the function will execute normally
     */
    #define Vector_index(__vec_ptr__, __value__, __boolean_comparator__, __result_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        bool __found__ = false; \
        size_t __i__ = 0; \
        for ( ; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
            if ((__boolean_comparator__)((*(__vec_ptr__))[__i__], (__value__))) { \
                __found__ = true; \
                break; \
            } \
        } \
        assert(__found__); \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __i__; } \
    } while (0)
#endif

/**
 * PUBLIC
 * 
 * gets the count of the value in the vector
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * size_t count = Vector_count(&vec, 5, boolean_comparator); // should return 0
 * ```
 */
#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * gets the count of the value in the vector
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @param __value__ [T] - the value to get the count of
     * @param __boolean_comparator__ [int (*)(T, T)] - the boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value given as an argument
     * @return [size_t] - the count of the value in the vector
     */
    #define Vector_count(__vec_ptr__, __value__, __boolean_comparator__) ({ \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        size_t __count__ = 0; \
        for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
            if ((__boolean_comparator__)((*(__vec_ptr__))[__i__], (__value__))) { \
                __count__++; \
            } \
        } \
        __count__; \
    })
#else
    /**
     * gets the count of the value in the vector
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @param __value__ [T] - the value to get the count of
     * @param __boolean_comparator__ [int (*)(T, T)] - the boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value given as an argument
     * @param __result_ptr__ [size_t*] - a pointer to the variable to store the count in, if NULL, the result will not be stored but the function will execute normally
     */
    #define Vector_count(__vec_ptr__, __value__, __boolean_comparator__, __result_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        size_t __count__ = 0; \
        for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
            if ((__boolean_comparator__)((*(__vec_ptr__))[__i__], (__value__))) { \
                __count__++; \
            } \
        } \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __count__; } \
    } while (0)
#endif

/**
 * PUBLIC
 * 
 * pushes a value to the end of the vector
 * @param __vec_ptr__ [T**] - a reference to the vector
 * @param __value__ [T] - the value to push to the vector
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @throw [assert] - if malloc fails
 * @example
 * ```
 * int *vec = Vector_init(int);
 * Vector_push(&vec, 1);
 * ```
 */
#define Vector_push(__vec_ptr__, __value__) do { \
    assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
    __vector_resize((__vec_ptr__)); \
    __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
    (*(__vec_ptr__))[__header__->length++] = (__value__); \
} while (0)

/**
 * PUBLIC
 * 
 * inserts a value at the specified index in the vector
 * @param __vec_ptr__ [T**] - a reference to the vector
 * @param __index__ [size_t] - the index to insert the value at
 * @param __value__ [T] - the value to insert
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @throw [assert] - if the index is out of bounds
 * @throw [assert] - if malloc fails
 * @example
 * ```
 * int *vec = Vector_init(int);
 * Vector_push(&vec, 1);
 * Vector_insert_at(&vec, 0, 2); // should insert 2 at the beginning of the vector [ 2, 1 ]
 * ```
 */
#define Vector_insert_at(__vec_ptr__, __index__, __value__) do { \
    assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
    __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
    assert((__index__) >= 0 && (__index__) <= __header__->length); \
    __vector_resize((__vec_ptr__)); \
    memmove((*(__vec_ptr__)) + (__index__) + 1, (*(__vec_ptr__)) + (__index__), (__header__->length - (__index__)) * __header__->element_size); \
    (*(__vec_ptr__))[(__index__)] = (__value__); \
    __header__->length++; \
} while (0)

/**
 * PUBLIC
 * 
 * inserts a value into the vector in sorted order
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @throw [assert] - if malloc fails
 * @example
 * ```
 * int *vec = Vector_init(int);
 * Vector_push(&vec, 1);
 * Vector_insert_sorted(&vec, 2, int_comparator); // should insert 2 at the end of the vector [ 1, 2 ]
 * ```
 */
#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * inserts a value into the vector in sorted order using binary search
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @param __value__ [T] - the value to insert
     * @param __ordering_comparator__ [int (*)(T, T)] - the ordering comparator function to compare the values
     * @return [size_t] - the index of the value inserted into the vector
     */
    #define Vector_insert_sorted(__vec_ptr__, __value__, __ordering_comparator__) ({ \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __vector_resize((__vec_ptr__)); \
        __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
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
#else
    /**
     * inserts a value into the vector in sorted order using binary search
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @param __value__ [T] - the value to insert
     * @param __ordering_comparator__ [int (*)(T, T)] - the ordering comparator function to compare the values
     * @param __result_ptr__ [size_t*] - a pointer to the variable to store the new index of the inserted value in, if NULL, the result will not be stored but the function will execute normally
     */
    #define Vector_insert_sorted(__vec_ptr__, __value__, __ordering_comparator__, __result_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __vector_resize((__vec_ptr__)); \
        __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
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
#endif

/**
 * PUBLIC
 * 
 * appends the values of the second vector to the first vector
 * @param __vec_ptr1__ [T**] - a reference to the first vector
 * @param __vec_ptr2__ [T**] - a reference to the second vector
 * @throw [assert] - if the reference to the first vector is NULL
 * @throw [assert] - if the first vector is NULL
 * @throw [assert] - if the reference to the second vector is NULL
 * @throw [assert] - if the second vector is NULL
 * @throw [assert] - if malloc fails
 * @example
 * ```
 * int *vec1 = Vector_init(int);
 * int *vec2 = Vector_init(int);
 * Vector_push(&vec1, 1);
 * Vector_push(&vec2, 2);
 * Vector_concat(&vec1, &vec2); // should append 2 to the end of vec1
 * ```
 */
#define Vector_concat(__vec_ptr1__, __vec_ptr2__) do { \
    assert(((__vec_ptr1__) != NULL) && ((*(__vec_ptr1__)) != NULL)); \
    assert(((__vec_ptr2__) != NULL) && ((*(__vec_ptr2__)) != NULL)); \
    for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr2__)); __i__++) { \
        Vector_push((__vec_ptr1__), (*(__vec_ptr2__))[__i__]); \
    } \
} while (0)

/**
 * PUBLIC
 * 
 * pops the last value from the vector
 * @param __vec_ptr__ [T**] - a reference to the vector
 * @return [T] - the value popped from the vector
 * @throw [assert] - if the vector is NULL
 * @throw [assert] - if malloc fails
 * @throw [assert] - if the vector is empty
 * @example
 * ```
 * int *vec = Vector_init(int);
 * Vector_push(&vec, 1);
 * int value = Vector_pop(&vec); // should return 1
 * ```
 */
#define Vector_pop(__vec_ptr__) (*(__vec_ptr__))[Vector_length((__vec_ptr__)) - 1]; do { \
    assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
    __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
    assert(__header__->length > 0); \
    __header__->length--; \
    __vector_resize((__vec_ptr__)); \
} while (0)

/**
 * PUBLIC
 * 
 * removes the value at the specified index from the vector
 * @param __vec_ptr__ [T**] - a reference to the vector
 * @param __index__ [size_t] - the index to remove the value from
 * @return [T] - the value removed from the vector
 * @throw [assert] - if the vector is NULL
 * @throw [assert] - if malloc fails
 * @throw [assert] - if the index is out of bounds
 * @example
 * ```
 * int *vec = Vector_init(int);
 * Vector_push(&vec, 1);
 * Vector_remove_at(&vec, 0); // should remove 1 from the vector
 * ```
 */
#define Vector_remove_at(__vec_ptr__, __index__) (*(__vec_ptr__))[(__index__)]; do { \
    assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
    __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
    assert((__index__) >= 0 && (__index__) < __header__->length); \
    memmove((*(__vec_ptr__)) + (__index__), (*(__vec_ptr__)) + (__index__) + 1, (__header__->length - (__index__) - 1) * __header__->element_size); \
    __header__->length--; \
    __vector_resize((__vec_ptr__)); \
} while (0)

/**
 * PUBLIC
 * 
 * removes the first occurrence of the value from the vector
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @throw [assert] - if malloc fails
 * @throw [assert] - if the value does not exist in the vector
 * @example
 * ```
 * int *vec = Vector_init(int);
 * for (int i = 0; i < 10; i++) { Vector_push(&vec, i); }
 * size_t index = Vector_remove_value(&vec, 5, boolean_comparator); // should remove 5 from the vector
 * ```
 */
#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * removes the first occurrence of the value from the vector
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @param __value__ [T] - the value to remove
     * @param __boolean_comparator__ [int (*)(T, T)] - the boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value to search for
     * @return [size_t] - the index of the value removed from the vector
     */
    #define Vector_remove_value(__vec_ptr__, __value__, __boolean_comparator__) ({ \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        size_t __index__ = Vector_index((__vec_ptr__), (__value__), (__boolean_comparator__)); \
        Vector_remove_at((__vec_ptr__), __index__); \
        __index__; \
    })
#else
    /**
     * removes the first occurrence of the value from the vector
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @param __value__ [T] - the value to remove
     * @param __boolean_comparator__ [int (*)(T, T)] - the boolean comparator function to compare the values, the first argument is the value in the vector, the second argument is the value to search for
     * @param __result_ptr__ [size_t*] - a pointer to the variable to store the index of the removed value before removing it, if NULL, the result will not be stored but the function will execute normally
     */
    #define Vector_remove_value(__vec_ptr__, __value__, __boolean_comparator__, __result_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        size_t __index__; \
        Vector_index((__vec_ptr__), (__value__), (__boolean_comparator__), &__index__); \
        Vector_remove_at((__vec_ptr__), (*(__result_ptr__))); \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __index__; } \
    } while (0)
#endif


/**
 * PUBLIC
 * 
 * clears the vector
 * @param __vec_ptr__ [T**] - a reference to the vector
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * Vector_push(&vec, 1); // [ 1 ]
 * Vector_clear(&vec); // should clear the vector [ ]
 * ```
 */
#define Vector_clear(__vec_ptr__) do { \
    assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
    __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
    __header__->length = 0; \
    __vector_resize((__vec_ptr__)); \
} while (0)

/**
 * PUBLIC
 * 
 * copies the vector
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @throw [assert] - if malloc fails
 * @note the returned result is a shallow copy, if the vector contains pointers to objects, the objects will not be copied
 * @example
 * ```
 * int *vec = Vector_init(int);
 * int *vec_copy = Vector_copy(&vec); // should create a copy of the vector
 * ```
 */
// the result is casted to void* to avoid the casting warning
#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * copies the vector
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @return [T*] - the copied vector
     */
    #define Vector_copy(__vec_ptr__) ({ \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __Vector_Header *__old_vec__ = __get_vector_header((__vec_ptr__)); \
        __Vector_Header *__new_vec__ = (__Vector_Header *)malloc(sizeof(__Vector_Header) + __old_vec__->capacity * __old_vec__->element_size); \
        assert(__new_vec__ != NULL); \
        memcpy(__new_vec__, __old_vec__, sizeof(__Vector_Header) + __old_vec__->length * __old_vec__->element_size); \
        (void*)__new_vec__->data; \
    })
#else
    /**
     * copies the vector
     * @param __old_vec_ptr__ [T**] - a reference to the vector
     * @param __new_vec_ptr__ [T**] - a reference to the pointer to the new vector
     * @throw [assert] - if the reference to the new vector is NULL
     */
    #define Vector_copy(__old_vec_ptr__, __new_vec_ptr__) do { \
        assert(((__old_vec_ptr__) != NULL) && ((*(__old_vec_ptr__)) != NULL)); \
        assert((__new_vec_ptr__) != NULL); \
        __Vector_Header *__old_vec__ = __get_vector_header((__old_vec_ptr__)); \
        __Vector_Header *__new_vec__ = (__Vector_Header *)malloc(sizeof(__Vector_Header) + __old_vec__->capacity * __old_vec__->element_size); \
        assert(__new_vec__ != NULL); \
        memcpy(__new_vec__, __old_vec__, sizeof(__Vector_Header) + __old_vec__->length * __old_vec__->element_size); \
        (*(__new_vec_ptr__)) = (void*)__new_vec__->data; \
    } while (0)
#endif

/**
 * PUBLIC
 * 
 * reverses the vector in place
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * Vector_reverse(&vec); // should reverse the vector
 * ```
 */
#if COMPILER_SUPPORTS_TYPEOF
    /**
     * reverses the vector in place
     * @param __vec_ptr__ [T**] - a reference to the vector
     */
    #define Vector_reverse(__vec_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
        for (size_t __i__ = 0; __i__ < __header__->length / 2; __i__++) { \
            typeof(**(__vec_ptr__)) __temp__ = (*(__vec_ptr__))[__i__]; \
            (*(__vec_ptr__))[__i__] = (*(__vec_ptr__))[__header__->length - __i__ - 1]; \
            (*(__vec_ptr__))[__header__->length - __i__ - 1] = __temp__; \
        } \
    } while (0)
#else
    /**
     * reverses the vector in place
     * @param __vec_ptr__ [T**] - a reference to the vector
     */
    #define Vector_reverse(__vec_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
        void *__temp__ = malloc(__header__->element_size); \
        assert(__temp__ != NULL); \
        for (size_t __i__ = 0; __i__ < __header__->length / 2; __i__++) { \
            memcpy(__temp__, (*(__vec_ptr__)) + __i__, __header__->element_size); \
            memcpy((*(__vec_ptr__)) + __i__, (*(__vec_ptr__)) + (__header__->length - __i__ - 1), __header__->element_size); \
            memcpy((*(__vec_ptr__)) + (__header__->length - __i__ - 1), __temp__, __header__->element_size); \
        } \
        free(__temp__); \
    } while (0)
#endif

/**
 * PUBLIC
 * 
 * sorts the vector in place
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * 
 * @example
 * ```
 * int *vec = Vector_init(int);
 * Vector_sort(&vec, int_comparator); // should sort the vector
 * ```
 */
#if COMPILER_SUPPORTS_TYPEOF
    #define __merge__(__arr__, __left_size__, __mid__, __right_size__, __ordering_comparator__) do { \
        int __i__, __j__, __k__; \
        int __n1__ = (__mid__) - (__left_size__) + 1; \
        int __n2__ =  (__right_size__) - (__mid__); \
        typeof(*(__arr__)) *__left_array__ = malloc(__n1__ * sizeof(*(__arr__))); \
        typeof(*(__arr__)) *__right_array__ = malloc(__n2__ * sizeof(*(__arr__))); \
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
     * sorts the vector in place
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @param __ordering_comparator__ [int (*)(T, T)] - the ordering comparator function to compare the values, should return a positive number if the first value is greater than the second value, a negative number if the first value is less than the second value, and 0 if the values are equal
     */
    #define Vector_sort(__vec_ptr__, __ordering_comparator__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
        __merge_sort__((*(__vec_ptr__)), __header__->length, (__ordering_comparator__)); \
    } while (0)
#else
    /**
     * sorts the vector in place
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @param __ordering_comparator__ [int (*)(T, T)] - the ordering comparator function to compare the values, should return a positive number if the first value is greater than the second value, a negative number if the first value is less than the second value, and 0 if the values are equal
     */
    #define __merge__(__arr__, __left_size__, __mid__, __right_size__, __ordering_comparator__, __vec_element_type__) do { \
        int __i__, __j__, __k__; \
        int __n1__ = (__mid__) - (__left_size__) + 1; \
        int __n2__ =  (__right_size__) - (__mid__); \
        __vec_element_type__ *__left_array__ = malloc(__n1__ * sizeof(*(__arr__))); \
        __vec_element_type__ *__right_array__ = malloc(__n2__ * sizeof(*(__arr__))); \
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
     * sorts the vector in place
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @param __ordering_comparator__ [int (*)(T, T)] - the ordering comparator function to compare the values, should return a positive number if the first value is greater than the second value, a negative number if the first value is less than the second value, and 0 if the values are equal
     */
    #define Vector_sort(__vec_ptr__, __ordering_comparator__, __vec_element_type__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
        __merge_sort__((*(__vec_ptr__)), __header__->length, (__ordering_comparator__), __vec_element_type__); \
    } while (0)
#endif

/**
 * PUBLIC
 * 
 * filters the vector
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @throw [assert] - if malloc fails
 * @example
 * ```
 * int *vec = Vector_init(int);
 * int *filtered_vec = Vector_filter(&vec, filter_function); // should return a filtered vector
 * ```
 */
#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * filters the vector
         * @param __vec_ptr__ [T**] - a reference to the vector
         * @param __filter__ [bool (*)(T)] - the filter function to filter the values
         * @return [T*] - a new filtered vector
         */
        #define Vector_filter(__vec_ptr__, __filter__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            typeof(*(__vec_ptr__)) __new_vec__ = Vector_init(typeof(**(__vec_ptr__))); \
            for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
                if (__filter__((*(__vec_ptr__))[__i__])) { \
                    Vector_push(&__new_vec__, (*(__vec_ptr__))[__i__]); \
                } \
            } \
            __new_vec__; \
        })
    #else
        /**
         * filters the vector
         * @param __vec_ptr__ [T**] - a reference to the vector
         * @param __filter__ [bool (*)(T)] - the filter function to filter the values
         * @param __vec_element_type__ [T] - the type of the elements of the returned vector
         * @return [T*] - a new filtered vector
         */
        #define Vector_filter(__vec_ptr__, __filter__, __vec_element_type__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            __vec_element_type__ *__new_vec__ = Vector_init(__vec_element_type__); \
            for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
                if ((__filter__)((*(__vec_ptr__))[__i__])) { \
                    Vector_push(&__new_vec__, (*(__vec_ptr__))[__i__]); \
                } \
            } \
            __new_vec__; \
        })
    #endif
#else
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * filters the vector
         * @param __vec_ptr__ [T**] - a reference to the vector
         * @param __filter__ [bool (*)(T)] - the filter function to filter the values
         * @param __new_vec_ptr__ [T**] - a reference to the pointer to the new vector
         * @throw [assert] - if the reference to the new vector is NULL
         */
        #define Vector_filter(__vec_ptr__, __filter__, __new_vec_ptr__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            assert((__new_vec_ptr__) != NULL); \
            (*(__new_vec_ptr__)) = Vector_init(typeof(**(__vec_ptr__))); \
            for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
                if ((__filter__)((*(__vec_ptr__))[__i__])) { \
                    Vector_push((__new_vec_ptr__), (*(__vec_ptr__))[__i__]); \
                } \
            } \
        } while (0)
    #else
        /**
         * filters the vector
         * @param __vec_ptr__ [T**] - a reference to the vector
         * @param __filter__ [bool (*)(T)] - the filter function to filter the values
         * @param __new_vec_ptr__ [T**] - a reference to the pointer to the new vector
         * @param __vec_element_type__ [T] - the type of the elements of the returned vector
         * @throw [assert] - if the reference to the new vector is NULL
         */
        #define Vector_filter(__vec_ptr__, __filter__, __new_vec_ptr__, __vec_element_type__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            assert((__new_vec_ptr__) != NULL); \
            (*(__new_vec_ptr__)) = Vector_init(__vec_element_type__); \
            for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
                if ((__filter__)((*(__vec_ptr__))[__i__])) { \
                    Vector_push((__new_vec_ptr__), (*(__vec_ptr__))[__i__]); \
                } \
            } \
        } while (0)
    #endif
#endif

/**
 * PUBLIC
 * 
 * applies a function to each value in the vector
 * @param __vec_ptr__ [T**] - a reference to the vector
 * @param __func__ [void (*)(T*)] - the function to apply to each value, it takes a pointer to the value
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * Vector_foreach(&vec, for_each_function); // should print each value in the vector
 * ```
 */
#define Vector_foreach(__vec_ptr__, __func__) do { \
    assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
    for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
        (__func__)(&((*(__vec_ptr__))[__i__])); \
    } \
} while (0)

/**
 * PUBLIC
 * 
 * maps a function to each value in the vector
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @throw [assert] - if malloc fails
 * @example
 * ```
 * int *vec = Vector_init(int);
 * int *vec_mapped = Vector_map(&vec, map_function); // should map each value in the vector
 * ```
 */
#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * maps a function to each value in the vector
         * @param __vec_ptr__ [T**] - a reference to the vector
         * @param __mapper__ [T (*)(T)] - the mapper function to map to each value
         * @return [T*] - a new mapped vector
         */
        #define Vector_map(__vec_ptr__, __mapper__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            typeof(*(__vec_ptr__)) __new_vec__ = Vector_init(typeof(**(__vec_ptr__))); \
            for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
                Vector_push(&__new_vec__, (__mapper__)((*(__vec_ptr__))[__i__])); \
            } \
            __new_vec__; \
        })
    #else
        /**
         * maps a function to each value in the vector
         * @param __vec_ptr__ [T**] - a reference to the vector
         * @param __mapper__ [T (*)(T)] - the mapper function to map to each value
         * @param __vec_element_type__ [T] - the type of the elements of the returned vector
         * @return [T*] - a new mapped vector
         */
        #define Vector_map(__vec_ptr__, __mapper__, __vec_element_type__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            __vec_element_type__ *__new_vec__ = Vector_init(__vec_element_type__); \
            for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
                Vector_push(&__new_vec__, (__mapper__)((*(__vec_ptr__))[__i__])); \
            } \
            __new_vec__; \
        })
    #endif
#else
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * maps a function to each value in the vector
         * @param __vec_ptr__ [T**] - a reference to the vector
         * @param __mapper__ [T (*)(T)] - the mapper function to map to each value
         * @param __new_vec_ptr__ [T**] - a reference to the new vector
         * @throw [assert] - if the reference to the new vector is NULL
         */
        #define Vector_map(__vec_ptr__, __mapper__, __new_vec_ptr__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            assert((__new_vec_ptr__) != NULL); \
            (*(__new_vec_ptr__)) = Vector_init(typeof(**(__vec_ptr__))); \
            for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
                Vector_push((__new_vec_ptr__), (__mapper__)((*(__vec_ptr__))[__i__])); \
            } \
        } while (0)
    #else
        /**
         * maps a function to each value in the vector
         * @param __vec_ptr__ [T**] - a reference to the vector
         * @param __mapper__ [T (*)(T)] - the mapper function to map to each value
         * @param __new_vec_ptr__ [T**] - a reference to the new vector
         * @param __vec_element_type__ [T] - the type of the elements of the returned vector
         * @throw [assert] - if the reference to the new vector is NULL
         */
        #define Vector_map(__vec_ptr__, __mapper__, __new_vec_ptr__, __vec_element_type__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            assert((__new_vec_ptr__) != NULL); \
            (*(__new_vec_ptr__)) = Vector_init(__vec_element_type__); \
            for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
                Vector_push((__new_vec_ptr__), (__mapper__)((*(__vec_ptr__))[__i__])); \
            } \
        } while (0)
    #endif
#endif

/**
 * PUBLIC
 * 
 * reduces the vector to a single value
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * int sum = Vector_reduce(&vec, lambda(int, (int acc, int x), { return acc + x; }), 0); // should reduce the vector
 * Vector_destroy(&vec);
 * ```
 */
#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * reduces the vector to a single value
         * @param __vec_ptr__ [T**] - a reference to the vector
         * @param __reducer__ [T (*)(T, T)] - the reducer function to reduce the values
         * @param __initial_value__ [T] - the initial value to start the reduction
         * @return [T] - the reduced value
         */
        #define Vector_reduce(__vec_ptr__, __reducer__, __initial_value__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            typeof((__initial_value__)) accumulator = (__initial_value__); \
            for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
                accumulator = (__reducer__)(accumulator, (*(__vec_ptr__))[__i__]); \
            } \
            accumulator; \
        })
    #else
        /**
         * reduces the vector to a single value
         * @param __vec_ptr__ [T**] - a reference to the vector
         * @param __reducer__ [T (*)(T, T)] - the reducer function to reduce the values
         * @param __initial_value__ [T] - the initial value to start the reduction
         * @param __accumulator_type__ [T] - the type of the accumulator
         */
        #define Vector_reduce(__vec_ptr__, __reducer__, __initial_value__, __accumulator_type__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            __accumulator_type__ accumulator = (__initial_value__); \
            for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
                accumulator = (__reducer__)(accumulator, (*(__vec_ptr__))[__i__]); \
            } \
            accumulator; \
        })
    #endif
#else
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * reduces the vector to a single value
         * @param __vec_ptr__ [T**] - a reference to the vector
         * @param __reducer__ [T (*)(T, T)] - the reducer function to reduce the values
         * @param __initial_value__ [T] - the initial value to start the reduction
         * @param __result_ptr__ [T*] - a reference to the variable to store the reduced value, if NULL, the result will not be stored but the function will execute normally
         */
        #define Vector_reduce(__vec_ptr__, __reducer__, __initial_value__, __result_ptr__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            typeof((__initial_value__)) __accumulator__ = (__initial_value__); \
            for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
                __accumulator__ = (__reducer__)(__accumulator__, (*(__vec_ptr__))[__i__]); \
            } \
            if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __accumulator__; } \
        } while (0)
    #else
        /**
         * reduces the vector to a single value
         * @param __vec_ptr__ [T**] - a reference to the vector
         * @param __reducer__ [T (*)(T, T)] - the reducer function to reduce the values
         * @param __initial_value__ [T] - the initial value to start the reduction
         * @param __result_ptr__ [T*] - a reference to the variable to store the result in, if NULL, the result will not be stored but the function will execute normally
         * @param __accumulator_type__ [T] - the type of the accumulator
         */
        #define Vector_reduce(__vec_ptr__, __reducer__, __initial_value__, __result_ptr__, __accumulator_type__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            __accumulator_type__ __accumulator__ = (__initial_value__); \
            for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
                __accumulator__ = (__reducer__)(__accumulator__, (*(__vec_ptr__))[__i__]); \
            } \
            if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __accumulator__; } \
        } while (0)
    #endif
#endif

/**
 * PUBLIC
 * 
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @note requires a compiler that supports 'typeof' and 'statement expressions'
 * @example
 * ```
 * int *vec = Vector_init(int);
 * bool any = Vector_any(&vec, lambda(bool, (int x), { return x == 5; })); // should return true
 * ```
 */
#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * checks if any value in the vector satisfies the function
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @param __func__ [bool (*)(T)] - the function to check if any value satisfies
     * @return [bool] - true if any value satisfies the function, false otherwise
     */
    #define Vector_any(__vec_ptr__, __func__) ({ \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        bool __any__ = false; \
        for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
            if ((__func__)((*(__vec_ptr__))[__i__])) { \
                __any__ = true; \
                break; \
            } \
        } \
        __any__; \
    })
#else
    /**
     * checks if any value in the vector satisfies the function
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @param __func__ [bool (*)(T)] - the function to check if any value satisfies
     * @param __result_ptr__ [bool*] - a reference to the variable to store the result in, if NULL, the result will not be stored but the function will execute normally
     */
    #define Vector_any(__vec_ptr__, __func__, __result_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        bool __any__ = false; \
        for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
            if ((__func__)((*(__vec_ptr__))[__i__])) { \
                __any__ = true; \
                break; \
            } \
        } \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __any__; } \
    } while (0)
#endif

/**
 * PUBLIC
 * 
 * returns true if all values in the vector satisfy the function
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @example
 * ```
 * int *vec = Vector_init(int);
 * bool all = Vector_all(&vec, lambda(bool, (int x), { return x < 10; })); // should return true
 * Vector_destroy(&vec);
 * ```
 */
#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    /**
     * checks if all values in the vector satisfy the function
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @param __func__ [bool (*)(T)] - the function to check if all values satisfy
     * @return [bool] - true if all values satisfy the function, false otherwise
     */
    #define Vector_all(__vec_ptr__, __func__) ({ \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        bool __all__ = true; \
        for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
            if (!(__func__)((*(__vec_ptr__))[__i__])) { \
                __all__ = false; \
                break; \
            } \
        } \
        __all__; \
    })
#else
    /**
     * checks if all values in the vector satisfy the function
     * @param __vec_ptr__ [T**] - a reference to the vector
     * @param __func__ [bool (*)(T)] - the function to check if all values satisfy
     * @param __result_ptr__ [bool*] - a reference to the variable to store the result in, if NULL, the result will not be stored but the function will execute normally
     */
    #define Vector_all(__vec_ptr__, __func__, __result_ptr__) do { \
        assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
        bool __all__ = true; \
        for (size_t __i__ = 0; __i__ < Vector_length((__vec_ptr__)); __i__++) { \
            if (!(__func__)((*(__vec_ptr__))[__i__])) { \
                __all__ = false; \
                break; \
            } \
        } \
        if ((__result_ptr__) != NULL) { (*(__result_ptr__)) = __all__; } \
    } while (0)
#endif

/**
 * PUBLIC
 * 
 * returns a slice of the vector from the start index to the end index with the specified step
 * @throw [assert] - if the reference to the vector is NULL
 * @throw [assert] - if the vector is NULL
 * @throw [assert] - if the start index is out of bounds
 * @throw [assert] - if the end index is out of bounds
 * @throw [assert] - if the step is less than or equal to 0
 * @example
 * ```
 * int *vec = Vector_init(int);
 * int *vec_sliced = Vector_slice(&vec, 0, 10, 2); // should throw an assert because 10 is out of bounds
 * ```
 */
#if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
    #if COMPILER_SUPPORTS_TYPEOF
        /**
         * returns a slice of the vector from the start index to the end index with the specified step
         * @param __vec_ptr__ [T**] - a reference to the vector
         * @param __start__ [size_t] - the start index of the slice
         * @param __end__ [size_t] - the end index of the slice
         * @param __step__ [size_t] - the step of the slice
         * @return [T*] - the sliced vector
         */
        #define Vector_slice(__vec_ptr__, __start__, __end__, __step__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            typeof(*(__vec_ptr__)) __new_vec__ = Vector_init(typeof(**(__vec_ptr__))); \
            __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
            assert((__start__) >= 0 && (__start__) < __header__->length); \
            assert((__end__) >= 0 && (__end__) <= __header__->length); \
            assert((__step__) > 0); \
            for (size_t __i__ = (__start__); __i__ < (__end__); __i__ += (__step__)) { \
                Vector_push(&__new_vec__, (*(__vec_ptr__))[__i__]); \
            } \
            __new_vec__; \
        })
    #else
        /**
         * returns a slice of the vector from the start index to the end index with the specified step
         * @param __vec_ptr__ [T**] - the pointer to the vector
         * @param __start__ [size_t] - the start index of the slice
         * @param __end__ [size_t] - the end index of the slice
         * @param __step__ [size_t] - the step of the slice
         * @param __vec_element_type__ [T] - the type of the elements of the returned vector
         */
        #define Vector_slice(__vec_ptr__, __start__, __end__, __step__, __vec_element_type__) ({ \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            __vec_element_type__ *__new_vec__ = Vector_init(__vec_element_type__); \
            __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
            assert((__start__) >= 0 && (__start__) < __header__->length); \
            assert((__end__) >= 0 && (__end__) <= __header__->length); \
            assert((__step__) > 0); \
            for (size_t __i__ = (__start__); __i__ < (__end__); __i__ += (__step__)) { \
                Vector_push(&__new_vec__, (*(__vec_ptr__))[__i__]); \
            } \
            __new_vec__; \
        })
    #endif
#else
    /**
     * returns a slice of the vector from the start index to the end index with the specified step
     * @param __vec_ptr__ [T**] - the pointer to the vector
     * @param __start__ [size_t] - the start index of the slice
     * @param __end__ [size_t] - the end index of the slice
     * @param __step__ [size_t] - the step of the slice
     * @param __new_vec_ptr__ [T**] - a reference to the pointer to the new vector
     * @throw [assert] - if the reference to the new vector is NULL
     */
    #if COMPILER_SUPPORTS_TYPEOF
        #define Vector_slice(__vec_ptr__, __start__, __end__, __step__, __new_vec_ptr__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            assert((__new_vec_ptr__) != NULL); \
            (*(__new_vec_ptr__)) = Vector_init(typeof(**(__vec_ptr__))); \
            __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
            assert((__start__) >= 0 && (__start__) < __header__->length); \
            assert((__end__) >= 0 && (__end__) <= __header__->length); \
            assert((__step__) > 0); \
            for (size_t __i__ = (__start__); __i__ < (__end__); __i__ += (__step__)) { \
                Vector_push((__new_vec_ptr__), (*(__vec_ptr__))[__i__]); \
            } \
        } while (0)
    #else
        /**
         * returns a slice of the vector from the start index to the end index with the specified step
         * @param __vec_ptr__ [T**] - the pointer to the vector
         * @param __start__ [size_t] - the start index of the slice
         * @param __end__ [size_t] - the end index of the slice
         * @param __step__ [size_t] - the step of the slice
         * @param __new_vec_ptr__ [T**] - a reference to the pointer to the new vector
         * @param __vec_element_type__ [T] - the type of the elements of the returned vector
         * @throw [assert] - if the reference to the new vector is NULL
         */
        #define Vector_slice(__vec_ptr__, __start__, __end__, __step__, __new_vec_ptr__, __vec_element_type__) do { \
            assert(((__vec_ptr__) != NULL) && ((*(__vec_ptr__)) != NULL)); \
            assert((__new_vec_ptr__) != NULL); \
            (*(__new_vec_ptr__)) = Vector_init(__vec_element_type__); \
            __Vector_Header *__header__ = __get_vector_header((__vec_ptr__)); \
            assert((__start__) >= 0 && (__start__) < __header__->length); \
            assert((__end__) >= 0 && (__end__) <= __header__->length); \
            assert((__step__) > 0); \
            for (size_t __i__ = (__start__); __i__ < (__end__); __i__ += (__step__)) { \
                Vector_push((__new_vec_ptr__), (*(__vec_ptr__))[__i__]); \
            } \
        } while (0)
    #endif
#endif

#ifdef __cplusplus // C++ support
}
#endif // C++ support

#endif // VECTOR_H
