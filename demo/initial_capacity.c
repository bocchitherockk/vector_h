#include <stdio.h>
#include "../vector.h"


void print_vec(int **vec_ptr) {
    printf("capacity = %zu,", Vector_get_capacity(vec_ptr));
    printf("\tinitial capacity = %zu,", Vector_get_initial_capacity(vec_ptr));
    printf("\tlength = %zu\n", Vector_get_length(vec_ptr));
}

int main()  {
    int *vec = Vector_init(int);
    print_vec(&vec);

    printf("changing initial capacity to 10\n");
    Vector_set_initial_capacity(&vec, 10);
    print_vec(&vec);

    for (int i = 0; i < 10; i++) {
        Vector_push(&vec, i);
    }
    print_vec(&vec);

    Vector_push(&vec, 10);
    print_vec(&vec);

    // this will change the capacity because the default optimal capacity calculation is based on the initial capacity
    Vector_set_initial_capacity(&vec, VECTOR_DEFAULT_INITIAL_CAPACITY);
    print_vec(&vec);

    Vector_destroy(&vec);
    return 0;
}
