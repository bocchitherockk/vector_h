#include <stdio.h>

#include "./vector.h"
#include "./modules/system_env/system_env.h"
#include "./modules/assertf/assertf.h"


__Vector_Header *__vector_get_header(void *vec_ptr) {
    void **temp_ptr = (void **)vec_ptr;
    assertf( temp_ptr != NULL, "ERROR: NULL is not a valid vector\n");
    assertf(*temp_ptr != NULL, "ERROR: Vector is NULL\n");
    return (__Vector_Header *)(((char *)*temp_ptr) - sizeof(__Vector_Header));
}

/**
 * Internal
 * 
 * Reallocates the memory of a vector
 * @param vec          [T*]     - The vector to reallocate
 * @param new_capacity [size_t] - The new capacity of the vector
 * @return             [void*]  - The new data of the vector
 * @throw              [assert] - If the vector is NULL, if malloc fails
 */
static void *__vector_realloc(void *vec_ptr, size_t new_capacity) {
    void **temp_ptr = (void **)vec_ptr;
    assertf( temp_ptr != NULL, "ERROR: NULL is not a valid vector\n");
    assertf(*temp_ptr != NULL, "ERROR: Vector is NULL\n");
    __Vector_Header *old_vec = __vector_get_header(vec_ptr);
    __Vector_Header *new_vec = (__Vector_Header *)malloc(sizeof(__Vector_Header) + new_capacity * old_vec->element_size);
    assertf(new_vec != NULL, "ERROR: Memory allocation failed\n");
    memcpy(new_vec, old_vec, sizeof(__Vector_Header) + old_vec->length * old_vec->element_size);
    new_vec->capacity = new_capacity;    
    free(old_vec);
    return new_vec->data;
}

#if COMPILER_SUPPORTS_BUILTIN_CLZ
    /**
     * Internal
     * 
     * Returns the optimal capacity for a vector given its length
     * @param length [size_t] - the length of the vector
     * @return       [size_t] - the optimal capacity for the vector
     */
    static size_t __vector_calculate_basic_optimal_capacity(void *vec_ptr) {
        __Vector_Header *header = __vector_get_header(vec_ptr);
        if (header->length < header->initial_capacity) { return header->initial_capacity; }
        size_t optimal_capacity = header->initial_capacity << (__builtin_clzl(header->initial_capacity) - __builtin_clzl(header->length));
        return optimal_capacity <= header->length ? optimal_capacity << 1 : optimal_capacity;
    }
#else // COMPILER_SUPPORTS_BUILTIN_CLZ
    /**
     * Internal
     * 
     * Returns the optimal capacity for a vector given its length
     * @param length [size_t] - the length of the vector
     * @return       [size_t] - the optimal capacity for the vector
     */
    static size_t __vector_calculate_basic_optimal_capacity(void *vec_ptr) {
        void **temp_ptr = (void**)vec_ptr;
        __Vector_Header *header = __vector_get_header(vec_ptr);
        if (header->length < header->initial_capacity) { return header->initial_capacity; }
        size_t optimal_capacity = header->initial_capacity;
        while (optimal_capacity <= header->length) { optimal_capacity <<= 1; }
        return optimal_capacity;
    }
#endif // COMPILER_SUPPORTS_BUILTIN_CLZ

void __vector_resize_if_needed(void *vec_ptr) {
    void **temp_ptr = (void**)vec_ptr;
    assertf( temp_ptr != NULL, "ERROR: NULL is not a valid vector\n");
    assertf(*temp_ptr != NULL, "ERROR: Vector is NULL\n");
    __Vector_Header *header = __vector_get_header(vec_ptr);
    size_t optimal_capacity = header->calculate_optimal_capacity_fn == NULL ? __vector_calculate_basic_optimal_capacity(vec_ptr) : header->calculate_optimal_capacity_fn(vec_ptr);
    if (optimal_capacity != header->capacity) {
        *temp_ptr = __vector_realloc(vec_ptr, optimal_capacity);
    }
}

void *__vector_init(size_t element_size) {
    __Vector_Header *header = (__Vector_Header *)malloc(sizeof(__Vector_Header) + element_size * VECTOR_DEFAULT_INITIAL_CAPACITY);
    assertf(header != NULL, "ERROR: Memory allocation failed\n");
    header->element_size = element_size;
    header->length = 0;
    header->capacity = VECTOR_DEFAULT_INITIAL_CAPACITY;
    header->initial_capacity = VECTOR_DEFAULT_INITIAL_CAPACITY;
    header->free_fn = NULL;
    header->calculate_optimal_capacity_fn = NULL;
    return header->data;
}

size_t Vector_get_element_size(void *vec_ptr) {
    void **temp_ptr = (void **)vec_ptr;
    assertf( temp_ptr != NULL, "ERROR: NULL is not a valid vector\n");
    assertf(*temp_ptr != NULL, "ERROR: Vector is NULL\n");
    return __vector_get_header(vec_ptr)->element_size;
}

size_t Vector_get_length(void *vec_ptr) {
    void **temp_ptr = (void **)vec_ptr;
    assertf( temp_ptr != NULL, "ERROR: NULL is not a valid vector\n");
    assertf(*temp_ptr != NULL, "ERROR: Vector is NULL\n");
    return __vector_get_header(temp_ptr)->length;
}

size_t Vector_get_capacity(void *vec_ptr) {
    void **temp_ptr = (void **)vec_ptr;
    assertf( temp_ptr != NULL, "ERROR: NULL is not a valid vector\n");
    assertf(*temp_ptr != NULL, "ERROR: Vector is NULL\n");
    return __vector_get_header(vec_ptr)->capacity;
}

size_t Vector_get_initial_capacity(void *vec_ptr) {
    void **temp_ptr = (void **)vec_ptr;
    assertf( temp_ptr != NULL, "ERROR: NULL is not a valid vector\n");
    assertf(*temp_ptr != NULL, "ERROR: Vector is NULL\n");
    return __vector_get_header(vec_ptr)->initial_capacity;
}

bool Vector_is_full(void *vec_ptr) {
    void **temp_ptr = (void **)vec_ptr;
    assertf( temp_ptr != NULL, "ERROR: NULL is not a valid vector\n");
    assertf(*temp_ptr != NULL, "ERROR: Vector is NULL\n");
    __Vector_Header *header = __vector_get_header(vec_ptr);
    return header->length == header->capacity;
}

bool Vector_is_underfilled(void *vec_ptr) {
    void **temp_ptr = (void **)vec_ptr;
    assertf( temp_ptr != NULL, "ERROR: NULL is not a valid vector\n");
    assertf(*temp_ptr != NULL, "ERROR: Vector is NULL\n");
    __Vector_Header *header = __vector_get_header(vec_ptr);
    return header->capacity > header->initial_capacity && header->length * 2 < header->capacity;
}

bool Vector_is_empty(void *vec_ptr) {
    void **temp_ptr = (void **)vec_ptr;
    assertf( temp_ptr != NULL, "ERROR: NULL is not a valid vector\n");
    assertf(*temp_ptr != NULL, "ERROR: Vector is NULL\n");
    return Vector_get_length(vec_ptr) == 0;
}

void Vector_set_initial_capacity(void *vec_ptr, size_t initial_capacity) {
    // this function calls __vector_resize_if_needed to immidiately resize the vector if it needs to (the default optimal capacity calculation relies on the initial capacity)
    void **temp_ptr = (void **)vec_ptr;
    assertf( temp_ptr != NULL, "ERROR: NULL is not a valid vector\n");
    assertf(*temp_ptr != NULL, "ERROR: Vector is NULL\n");
    __vector_get_header(vec_ptr)->initial_capacity = initial_capacity;
    __vector_resize_if_needed(vec_ptr);
}

void Vector_set_free_fn(void *vec_ptr, Vector_free_fn free_fn) {
    void **temp_ptr = (void **)vec_ptr;
    assertf( temp_ptr != NULL, "ERROR: NULL is not a valid vector\n");
    assertf(*temp_ptr != NULL, "ERROR: Vector is NULL\n");
    __vector_get_header(vec_ptr)->free_fn = free_fn;
}

void Vector_set_calculate_optimal_capacity_fn(void *vec_ptr, Vector_calculate_optimal_capacity_fn calculate_optimal_capacity_fn) {
    void **temp_ptr = (void **)vec_ptr;
    assertf( temp_ptr != NULL, "ERROR: NULL is not a valid vector\n");
    assertf(*temp_ptr != NULL, "ERROR: Vector is NULL\n");
    __vector_get_header(vec_ptr)->calculate_optimal_capacity_fn = calculate_optimal_capacity_fn;
}
