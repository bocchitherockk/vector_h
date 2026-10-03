#include <stdio.h>
#include <assert.h>

#include "./vector.h"
#include "./modules/system_env/system_env.h"
#include "./modules/assertf/assertf.h"


/**
 * Internal
 * 
 * Returns a pointer to the header of a vector
 * @param vec_ptr [T**]              - A reference to the vector
 * @return        [__Vector_Header*] - A pointer to the header of the vector
 * @throw         [assert]           - If the vector is NULL
 */
__Vector_Header *__vector_get_header(void *vec_ptr) {
    // I use assert here instead of assertf because i want it to be inlined as much as possible (i havent' actually tested anything, i just assumed)
    void **temp_ptr = (void **)vec_ptr;
    assert(*temp_ptr != NULL && "ERROR: Vector is NULL\n");
    return (__Vector_Header *)(((char *)*temp_ptr) - sizeof(__Vector_Header));
}

/**
 * Internal
 * 
 * Reallocates the memory of a vector
 * @param vec_ptr      [T**]    - A reference to the vector
 * @param new_capacity [size_t] - The new capacity of the vector
 * @return             [void*]  - The new data of the vector
 * @throw              [assert] - If the vector is NULL
 * @throw              [assert] - If malloc fails
 */
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
    /**
     * Internal
     * 
     * Returns the optimal capacity for a vector given its length
     * @param vec_ptr [T**]    - A reference to the vector
     * @return        [size_t] - the optimal capacity for the vector
     * @throw         [assert] - If the vector is NULL
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
     * @param vec_ptr [T**]    - A reference to the vector
     * @return        [size_t] - the optimal capacity for the vector
     * @throw         [assert] - If the vector is NULL
     */
    static size_t __vector_calculate_basic_optimal_capacity(void *vec_ptr) {
        __Vector_Header *header = __vector_get_header(vec_ptr);
        if (header->length < header->initial_capacity) { return header->initial_capacity; }
        size_t optimal_capacity = header->initial_capacity;
        while (optimal_capacity <= header->length) { optimal_capacity <<= 1; }
        return optimal_capacity;
    }
#endif // COMPILER_SUPPORTS_BUILTIN_CLZ

/**
 * Internal
 * 
 * Resizes the vector if the capacity is not optimal
 * @param vec_ptr [T**]    - A reference to the vector
 * @throw         [assert] - If the vector is NULL
 */
void __vector_resize_if_needed(void *vec_ptr) {
    __Vector_Header *header = __vector_get_header(vec_ptr);
    size_t optimal_capacity = header->calculate_optimal_capacity_fn == NULL ? __vector_calculate_basic_optimal_capacity(vec_ptr) : header->calculate_optimal_capacity_fn(vec_ptr);
    if (optimal_capacity != header->capacity) {
        *(void**)vec_ptr = __vector_realloc(vec_ptr, optimal_capacity);
    }
}

/**
 * Internal
 * 
 * Initializes a vector
 * @param element_size       [size_t]               - The size of the vector type
 * @param vector_init_params [__Vector_Init_Params] - The optional parameters for initializing the vector
 * @return                   [T*]                   - The array of data
 * @throw                    [assert]               - If malloc fails
 */
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

/**
 * Public
 * 
 * Returns the size of an element in the vector
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [size_t] - The element size of the vector
 * @throw         [assert] - If the vector is NULL
 */
size_t Vector_get_element_size(void *vec_ptr) {
    return __vector_get_header(vec_ptr)->element_size;
}

/**
 * Public
 * 
 * Returns the number of elements in a vector
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [size_t] - The number of elements in the vector
 * @throw         [assert] - If the vector is NULL
 */
size_t Vector_get_length(void *vec_ptr) {
    return __vector_get_header(vec_ptr)->length;
}

/**
 * Public
 * 
 * Returns the capacity of the vector
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [size_t] - The capacity of the vector
 * @throw         [assert] - If the vector is NULL
 */
size_t Vector_get_capacity(void *vec_ptr) {
    return __vector_get_header(vec_ptr)->capacity;
}

/**
 * Public
 * 
 * Returns the initial capacity of the vector
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [size_t] - The initial capacity of the vector
 * @throw         [assert] - If the vector is NULL
 */
size_t Vector_get_initial_capacity(void *vec_ptr) {
    return __vector_get_header(vec_ptr)->initial_capacity;
}

/**
 * Public
 * 
 * Checks if the vector is full
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [bool]   - True if the vector is full, false otherwise
 * @throw         [assert] - If the vector is NULL
 */
bool Vector_is_full(void *vec_ptr) {
    __Vector_Header *header = __vector_get_header(vec_ptr);
    return header->length == header->capacity;
}

/**
 * Public
 * 
 * Checks if the vector is underfilled
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [bool]   - True if the vector is underfilled, false otherwise
 * @throw         [assert] - If the vector is NULL
 */
bool Vector_is_underfilled(void *vec_ptr) {
    __Vector_Header *header = __vector_get_header(vec_ptr);
    return header->capacity > header->initial_capacity && header->length * 2 < header->capacity;
}

/**
 * Public
 * 
 * Checks if the vector is empty
 * @param vec_ptr [T**]    - A reference to the vector
 * @return        [bool]   - True if the vector is empty, false otherwise
 * @throw         [assert] - If the vector is NULL
 */
bool Vector_is_empty(void *vec_ptr) {
    return Vector_get_length(vec_ptr) == 0;
}

/**
 * Public
 * 
 * Sets the initial capacity of the vector
 * @param vec_ptr          [T**]    - A reference to the vector
 * @param initial_capacity [size_t] - The initial capacity to set
 * @throw                  [assert] - If the vector is NULL
 */
void Vector_set_initial_capacity(void *vec_ptr, size_t initial_capacity) {
    // This function calls __vector_resize_if_needed to immidiately resize the vector if it needs to (the default optimal capacity calculation relies on the initial capacity)
    // This is a fine approach because if the user is sane, they will call this function only once in the lifetime of the vector if not call it at all
    // which I find it better than overcomplicating the API and making Vector_init take an initial capacity argument.
    // However, i might consider using optional arguments trick in the future to make Vector_init take optional arguments, but for now, this is fine. (hopefully i don't do that without forgetting to change this comment :D)
    __vector_get_header(vec_ptr)->initial_capacity = initial_capacity;
    __vector_resize_if_needed(vec_ptr);
}

/**
 * Public
 * 
 * Sets the free function for the vector
 * @param vec_ptr [T**]            - A reference to the vector
 * @param free_fn [Vector_free_fn] - The free function to set
 * @throw         [assert]         - If the vector is NULL
 */
void Vector_set_free_fn(void *vec_ptr, Vector_free_fn free_fn) {
    __vector_get_header(vec_ptr)->free_fn = free_fn;
}

/**
 * Public
 * 
 * Sets the function to calculate the optimal capacity for the vector
 * @param vec_ptr                       [T**]                                  - A reference to the vector
 * @param calculate_optimal_capacity_fn [Vector_calculate_optimal_capacity_fn] - The function to set
 * @throw                               [assert]                               - If the vector is NULL
 */
void Vector_set_calculate_optimal_capacity_fn(void *vec_ptr, Vector_calculate_optimal_capacity_fn calculate_optimal_capacity_fn) {
    __vector_get_header(vec_ptr)->calculate_optimal_capacity_fn = calculate_optimal_capacity_fn;
}
