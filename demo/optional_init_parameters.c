#include <stdio.h>
#include <assert.h>

#include "../vector.h"

#define NEW_INITiAL_CAPACITY 10

void free_fn(void *vec_ptr) {
    __Vector_Header *header = __vector_get_header(vec_ptr);
    printf("Freeing vector with length: %zu\n", header->length);
    free(header);
}

int main(void) {
    int *vec1 = Vector_init(int);
    int *vec2 = Vector_init(int, .initial_capacity = NEW_INITiAL_CAPACITY);
    int *vec3 = Vector_init(int, .free_fn = free_fn);
    int *vec4 = Vector_init(int, .initial_capacity = NEW_INITiAL_CAPACITY, .free_fn = free_fn);
    int *vec5 = Vector_init(int, .free_fn = free_fn, .initial_capacity = NEW_INITiAL_CAPACITY); // Testing order of parameters

    // vec1
    assert(Vector_get_element_size(&vec1) == sizeof(int));
    assert(Vector_get_length(&vec1) == 0);
    assert(Vector_get_capacity(&vec1) == VECTOR_DEFAULT_INITIAL_CAPACITY);
    assert(Vector_get_initial_capacity(&vec1) == VECTOR_DEFAULT_INITIAL_CAPACITY);
    assert(__vector_get_header(&vec1)->free_fn == NULL);
    assert(__vector_get_header(&vec1)->calculate_optimal_capacity_fn == NULL);

    // vec2
    assert(Vector_get_element_size(&vec2) == sizeof(int));
    assert(Vector_get_length(&vec2) == 0);
    assert(Vector_get_capacity(&vec2) == NEW_INITiAL_CAPACITY);
    assert(Vector_get_initial_capacity(&vec2) == NEW_INITiAL_CAPACITY);
    assert(__vector_get_header(&vec2)->free_fn == NULL);
    assert(__vector_get_header(&vec2)->calculate_optimal_capacity_fn == NULL);


    // vec3
    assert(Vector_get_element_size(&vec3) == sizeof(int));
    assert(Vector_get_length(&vec3) == 0);
    assert(Vector_get_capacity(&vec3) == VECTOR_DEFAULT_INITIAL_CAPACITY);
    assert(Vector_get_initial_capacity(&vec3) == VECTOR_DEFAULT_INITIAL_CAPACITY);
    assert(__vector_get_header(&vec3)->free_fn == free_fn);
    assert(__vector_get_header(&vec3)->calculate_optimal_capacity_fn == NULL);

    // vec4
    assert(Vector_get_element_size(&vec4) == sizeof(int));
    assert(Vector_get_length(&vec4) == 0);
    assert(Vector_get_capacity(&vec4) == NEW_INITiAL_CAPACITY);
    assert(Vector_get_initial_capacity(&vec4) == NEW_INITiAL_CAPACITY);
    assert(__vector_get_header(&vec4)->free_fn == free_fn);
    assert(__vector_get_header(&vec4)->calculate_optimal_capacity_fn == NULL);

    // vec5
    assert(Vector_get_element_size(&vec5) == sizeof(int));
    assert(Vector_get_length(&vec5) == 0);
    assert(Vector_get_capacity(&vec5) == NEW_INITiAL_CAPACITY);
    assert(Vector_get_initial_capacity(&vec5) == NEW_INITiAL_CAPACITY);
    assert(__vector_get_header(&vec5)->free_fn == free_fn);
    assert(__vector_get_header(&vec5)->calculate_optimal_capacity_fn == NULL);

    Vector_destroy(&vec1);
    Vector_destroy(&vec2);
    Vector_destroy(&vec3);
    Vector_destroy(&vec4);
    Vector_destroy(&vec5);

    printf("All tests passed successfully!\n");
    return 0;
}
