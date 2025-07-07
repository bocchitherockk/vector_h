#include <stdio.h>
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
    print_vec(&vec);

    for (int i = 0; i < 10; i++) {
        Vector_push(&vec, i);
        print_vec(&vec);
    }

    printf("*******************\n");
    for (int i = 0; i < 10; i++) {
        Vector_pop(&vec);
        print_vec(&vec);
    }

    Vector_destroy(&vec);
    return 0;
}
