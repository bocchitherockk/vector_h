#include "./vector.h"
#include "./modules/system_env/system_env.h"

__Vector_Header *__get_vector_header(void *vec) {
    assert(vec != NULL);
    return vec - sizeof(__Vector_Header);
}
/**
 * INTERNAL
 * 
 * reallocates the memory of a vector
 * @param vec [T*] - the vector to reallocate
 * @param new_capacity [size_t] - the new capacity of the vector
 * @return [void*] - the new data of the vector
 * @throw [assert] - if the vector is NULL, if malloc fails
 */
static void *__vector_realloc(void *vec, size_t new_capacity) {
    assert(vec != NULL);
    __Vector_Header *old_vec = __get_vector_header(vec);
    __Vector_Header *new_vec = malloc(sizeof(__Vector_Header) + new_capacity * old_vec->element_size);
    assert(new_vec != NULL);
    memcpy(new_vec, old_vec, sizeof(__Vector_Header) + old_vec->length * old_vec->element_size);
    new_vec->capacity = new_capacity;    
    free(old_vec);
    return new_vec->data;
}


/**
 * INTERNAL
 * 
 * returns the optimal capacity for a vector given its length
 * @param length [size_t] - the length of the vector
 * @return [size_t] - the optimal capacity for the vector
 */
#if COMPILER_SUPPORTS___BUILTIN_CLZ
    static size_t __vector_get_optimal_capacity(size_t length) {
        if (length < VECTOR_INITIAL_CAPACITY) { return VECTOR_INITIAL_CAPACITY; }
        size_t optimal_capacity = VECTOR_INITIAL_CAPACITY << (__builtin_clzl(VECTOR_INITIAL_CAPACITY) - __builtin_clzl(length));
        return optimal_capacity <= length ? optimal_capacity << 1 : optimal_capacity;
    }
#else
    static size_t __vector_get_optimal_capacity(size_t length) {
        if (length < VECTOR_INITIAL_CAPACITY) { return VECTOR_INITIAL_CAPACITY; }
        size_t optimal_capacity = VECTOR_INITIAL_CAPACITY;
        while (optimal_capacity <= length) { optimal_capacity <<= 1; }
        return optimal_capacity;
    }
#endif

void __vector_resize(void **vec_ptr) {
    __Vector_Header *header = __get_vector_header(*vec_ptr);
    size_t optimal_capacity = __vector_get_optimal_capacity(header->length);
    if (optimal_capacity != header->capacity) {
        *vec_ptr = __vector_realloc(*vec_ptr, optimal_capacity);
    }
}

void *__vector_init(size_t element_size) {
    __Vector_Header *header = malloc(sizeof(__Vector_Header) + element_size * VECTOR_INITIAL_CAPACITY);
    assert(header != NULL);
    header->element_size = element_size;
    header->length = 0;
    header->capacity = VECTOR_INITIAL_CAPACITY;
    return header->data;
}

size_t Vector_element_size(void *vec) {
    assert(vec != NULL);
    return __get_vector_header(vec)->element_size;
}

size_t Vector_length(void *vec) {
    assert(vec != NULL);
    return __get_vector_header(vec)->length;
}

size_t Vector_capacity(void *vec) {
    assert(vec != NULL);
    return __get_vector_header(vec)->capacity;
}

bool Vector_is_full(void *vec) {
    assert(vec != NULL);
    __Vector_Header *header = __get_vector_header(vec);
    return header->length == header->capacity;
}

bool Vector_is_underfilled(void *vec) {
    assert(vec != NULL);
    __Vector_Header *header = __get_vector_header(vec);
    return header->capacity > VECTOR_INITIAL_CAPACITY && header->length * 2 < header->capacity;
}

bool Vector_is_empty(void *vec) {
    assert(vec != NULL);
    return Vector_length(vec) == 0;
}
