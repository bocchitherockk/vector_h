#include <stdio.h>
#include <assert.h>
#define VECTOR_IMPLEMENTATION
#include "../vector.h"


void print_vec(int **vec_ptr) {
    printf("capacity = %zu,", Vector_get_capacity(vec_ptr));
    printf("\tinitial capacity = %zu,", Vector_get_initial_capacity(vec_ptr));
    printf("\tlength = %zu\n", Vector_get_length(vec_ptr));
}

int main()  {
    int *vec = Vector_init(int);
    assert(Vector_get_initial_capacity(&vec) == VECTOR_DEFAULT_INITIAL_CAPACITY);

    Vector_set_initial_capacity(&vec, 10);
    assert(Vector_get_initial_capacity(&vec) == 10);

    for (int i = 0; i < 10; i++) {
        Vector_push(&vec, i);
    }
    assert(Vector_get_capacity(&vec) == 10);

    Vector_push(&vec, 10);
    assert(Vector_get_capacity(&vec) == 20);

    // this will change the capacity because the default optimal capacity calculation is based on the initial capacity
    Vector_set_initial_capacity(&vec, VECTOR_DEFAULT_INITIAL_CAPACITY);
    assert(Vector_get_initial_capacity(&vec) == VECTOR_DEFAULT_INITIAL_CAPACITY);
    assert(Vector_get_capacity(&vec) == 16);

    Vector_destroy(&vec);

    printf("All tests passed successfully!\n");
    return 0;
}
