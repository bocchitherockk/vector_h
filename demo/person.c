#include <stdio.h>
#include <assert.h>
#define VECTOR_IMPLEMENTATION
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

bool equal_vec_arr_person(Person *vec, Person arr[], size_t arr_length) {
    if (Vector_get_length(&vec) != arr_length) {
        return false;
    }
    for (size_t i = 0; i < arr_length; i++) {
        if (strcmp(vec[i].name, arr[i].name) != 0 || vec[i].age != arr[i].age) {
            return false;
        }
    }
    return true;
}

bool equal_vec_array_int(int *vec, int arr[], size_t arr_length) {
    if (Vector_get_length(&vec) != arr_length) {
        return false;
    }
    for (size_t i = 0; i < arr_length; i++) {
        if (vec[i] != arr[i]) {
            return false;
        }
    }
    return true;
}

int mapper(Person p) {
    return p.age;
}

int main(void) {
    Person *vec = Vector_init(Person);

    Vector_push(&vec, ((Person){ "Alice", 30 }));
    Vector_push(&vec, ((Person){ "Bob", 25 }));
    Vector_push(&vec, ((Person){ "Charlie", 35 }));
    assert(
        equal_vec_arr_person(vec, (Person[]){
            {"Alice", 30},
            {"Bob", 25},
            {"Charlie", 35}
        }, 3)
        == true
    );

    int *ages = Vector_map(&vec, mapper, int);
    assert(
        equal_vec_array_int(ages, (int[]){30, 25, 35}, 3)
        == true
    );

    Vector_destroy(&vec);
    Vector_destroy(&ages);

    printf("All tests passed successfully!\n");
    return 0;
}
