#include <stdio.h>
#include <assert.h>
#define VECTOR_IMPLEMENTATION
#include "../vector.h"


void print_vec(int **vec_ptr) {
    printf("capacity = %zu,", Vector_get_capacity(vec_ptr));
    printf("\tlength = %zu\n", Vector_get_length(vec_ptr));
}

size_t custom_calculate_optimal_capacity(void *vec_ptr) {
    size_t length = Vector_get_length(vec_ptr);
    size_t capacity = Vector_get_capacity(vec_ptr);
    if (length == capacity) return capacity + 2;
    if (capacity - length > 2) return capacity - 2;
    return capacity;
}

int main(void) {
    int *vec = Vector_init(int);
    Vector_set_calculate_optimal_capacity_fn(&vec, custom_calculate_optimal_capacity);

    for (int i = 0; i < 10; i++) {
        Vector_push(&vec, i);
    }
    assert(Vector_get_capacity(&vec) == 10);

    // length = 11; capacity = 12
    Vector_push(&vec, 1);
    assert(Vector_get_capacity(&vec) == 12);

    // length = 10; capacity = 12
    Vector_pop(&vec);
    assert(Vector_get_capacity(&vec) == 12);

    // length = 9; capacity = 10
    Vector_pop(&vec);
    assert(Vector_get_capacity(&vec) == 10);

    Vector_destroy(&vec);

    printf("All tests passed successfully!\n");
    return 0;
}
