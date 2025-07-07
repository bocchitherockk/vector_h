#include <stdio.h>
#include <time.h>
#include "../vector.h"

typedef struct person {
    char name[50];
    int age;
} Person;

void print_vector_int(int *vec) {
    printf("{\n");
    printf("    element_size: %ld\n", Vector_get_element_size(&vec));
    printf("    length: %ld\n", Vector_get_length(&vec));
    printf("    capacity: %ld\n", Vector_get_capacity(&vec));
    printf("    data: [");
    for (size_t i = 0; i < Vector_get_length(&vec); i++) {
        printf("%d, ", vec[i]);
    }
    printf("]\n}\n");
}

void print_vector_person(Person *vec) {
    printf("{\n");
    printf("    element_size: %ld\n", Vector_get_element_size(&vec));
    printf("    length: %ld\n", Vector_get_length(&vec));
    printf("    capacity: %ld\n", Vector_get_capacity(&vec));
    printf("    data: [\n");
    for (size_t i = 0; i < Vector_get_length(&vec); i++) {
        printf("        { name: %s, age: %d }\n", vec[i].name, vec[i].age);
    }
    printf("    ]\n}\n");
}


int mapper(Person p) {
    return p.age;
}

int main(void) {
    srand(time(NULL));
    Person *vec = Vector_init(Person);
    for (int i = 0; i < 20; i++) {
        Person p = { .name = "John Doe", .age = rand() % 100 };
        Vector_push(&vec, p);
    }
    print_vector_person(vec);

    int *ages = Vector_map(&vec, mapper, int);
    print_vector_int(ages);

    Vector_destroy(&vec);
    Vector_destroy(&ages);
    return 0;
}
