#include <stdio.h>
#include <vector.h>


void print_vec(int **vec_ptr) {
    printf("capacity = %zu,", Vector_capacity(vec_ptr));
    printf("\tlength = %zu\n", Vector_length(vec_ptr));
}

size_t get_optimal_capacity(void *vec_ptr) {
    void **temp_ptr = (void**)vec_ptr;
    __Vector_Header *header = __get_vector_header(temp_ptr);
    if (header->length == header->capacity) {
        return header->capacity + 2;
    }
    if (header->capacity - header->length > 2) {
        return header->capacity - 2;
    }
    return header->capacity;
}

int main() {
    int *vec = Vector_init(int);
    Vector_set_optimal_capacity_fn(&vec, get_optimal_capacity);
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