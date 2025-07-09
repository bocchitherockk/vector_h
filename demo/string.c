#include <stdio.h>
#include "../vector.h"

void print_vector(char ***vec) {
    size_t vec_length = Vector_get_length(vec);
    printf("{\n");
    printf("    element_size: %ld,\n", Vector_get_element_size(vec));
    printf("    length: %ld,\n", vec_length);
    printf("    capacity: %ld,\n", Vector_get_capacity(vec));
    printf("    data: [");
    for (int i = 0; i < vec_length; i++) {
        printf("\"%s\"%s", (*vec)[i], vec_length - 1 == i ? "" : ", ");
    }
    printf("]\n");
    printf("}\n");
}


int main(void) {
    char hello[]       = "hello";
    char world[]       = "world";
    char exclamation[] = "!";
    char yassine[]     = "yassine";
    char *hamza        = "hamza"; // this one is read-only due to the nature of C

    char **vec = Vector_init(char *);
    Vector_push(&vec, hello);
    Vector_push(&vec, world);
    Vector_push(&vec, exclamation);
    Vector_push(&vec, yassine);
    Vector_push(&vec, hamza);
    print_vector(&vec);

    vec[0][0] =  'S';
    print_vector(&vec);

    return 0;
}
