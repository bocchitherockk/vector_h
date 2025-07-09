#include <stdio.h>
#include "../vector.h"

void print_vector(int ***vec) {
    size_t vec_length = Vector_get_length(vec);
    printf("{\n");
    printf("    element_size: %ld,\n", Vector_get_element_size(vec));
    printf("    length: %ld,\n", vec_length);
    printf("    capacity: %ld,\n", Vector_get_capacity(vec));
    printf("    data: [\n");
    for (int i = 0; i < vec_length; i++) {
        int *v = (*vec)[i];
        size_t v_length = Vector_get_length(&v);
        printf("        {\n");
        printf("            element_size: %ld,\n", Vector_get_element_size(&v));
        printf("            length: %ld,\n", v_length);
        printf("            capacity: %ld,\n", Vector_get_capacity(&v));
        printf("            data: [");
        for (int j = 0; j < Vector_get_length(&v); j++) {
            printf("%d%s", v[j], j == v_length - 1 ? "" : ", ");
        }
        printf("]\n");
        printf("        }%s\n", i == vec_length - 1 ? "" : ",");
    }
    printf("    ]\n");
    printf("}\n");
}

void free_fn(void *vec_ptr) {
    int ***temp_vec = (int ***)vec_ptr;
    for (int i = 0; i < Vector_get_length(temp_vec); i++) {
        int *v = (*temp_vec)[i];
        Vector_destroy(&v);
    }
    free(__vector_get_header(temp_vec));
}

int main(void) {
    int **vec_of_vec = Vector_init(int *);
    Vector_set_free_fn(&vec_of_vec, free_fn);

    for (int i = 0; i < 3; i++)
        Vector_push(&vec_of_vec, Vector_init(int));

    // resize each inner vector by increasing its capacity
    // make sure that everything works fine and the pointers are valid
    for (int i =  0; i < 15; i++)
        Vector_push(&vec_of_vec[0], i);
    for (int i = 15; i < 30; i++)
        Vector_push(&vec_of_vec[1], i);
    for (int i = 30; i < 45; i++)
        Vector_push(&vec_of_vec[2], i);

    print_vector(&vec_of_vec);

    // resize vec by increasing the capacity
    // make sure that everything works fine and the pointers are still valid
    Vector_push(&vec_of_vec, Vector_init(int));
    Vector_push(&vec_of_vec, Vector_init(int));
    print_vector(&vec_of_vec);

    Vector_destroy(&vec_of_vec);
    return 0;
}
