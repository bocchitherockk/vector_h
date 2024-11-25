#include <stdio.h>
#include <time.h>
#include <vector.h>

void print_vector_int(int *vec) {
    printf("{\n");
    printf("    element_size: %ld\n", Vector_element_size(&vec));
    printf("    length: %ld\n", Vector_length(&vec));
    printf("    capacity: %ld\n", Vector_capacity(&vec));
    printf("    data: [");
    if (Vector_length(&vec) != 0) {
        printf("%d", vec[0]);
        for (size_t i = 1; i < Vector_length(&vec); i++) {
            printf(", %d", vec[i]);
        }
    }
    printf("]\n}\n");
}

// selects the numbers that are equal to the given value
int int_boolean_comparator(int a, int b) { return a == b; }
// sorts in ascending order
int int_ordering_comparator(int a, int b) { return a - b; }
// selects even numbers
bool filter(int value) { return value % 2 == 0; }
// doubles the value
void for_each(int *value) { *value *= 2; }
// adds 2 to the value
int map(int x) { return x + 2; }
// calculates the sum
int sum_reducer(int acc, int x) { return acc + x; }
// checks if the value is equal to 10
bool any_all(int value) { return value == 10; }
// sets the value to 10
void set_all_to_10(int *value) { *value = 10; }


int main() {
    printf("COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS: %s\n", COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS ? "true" : "false");
    printf("COMPILER_SUPPORTS_TYPEOF: %s\n", COMPILER_SUPPORTS_TYPEOF ? "true" : "false");
    printf("COMPILER_SUPPORTS_BUILTIN_CLZ: %s\n", COMPILER_SUPPORTS_BUILTIN_CLZ ? "true" : "false");
    srand(time(NULL));

    printf("initializing vector vec1 ...\n");
    int *vec1 = Vector_init(int);

    printf("vector vec1 initialized\n");
    printf("vec1 : ");
    print_vector_int(vec1);

    printf("pushing 20 random numbers to vec1 ...\n");
    for (int i = 0; i < 20; i++) {
        Vector_push(&vec1, rand() % 10);
    }
    printf("20 random numbers pushed to vec1\n");
    printf("vec1 : ");
    print_vector_int(vec1);

    printf("getting the index of the first 5 in vec1 ...\n");
    #if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
        int index = Vector_index(&vec1, 5, int_boolean_comparator);
    #else
        int index = 0;
        Vector_index(&vec1, 5, int_boolean_comparator, &index);
    #endif
    printf("index of the first 5 in vec1: %d\n", index);

    printf("getting the count of 5s in vec1 ...\n");

    #if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
        int count = Vector_count(&vec1, 5, int_boolean_comparator);
    #else
        int count = 0;
        Vector_count(&vec1, 5, int_boolean_comparator, &count);
    #endif
    printf("count of 5s in vec1: %d\n", count);


    printf("inserting 100 at index 5 in vec1 ...\n");
    Vector_insert_at(&vec1, 5, 100);
    printf("100 inserted at index 5 in vec1\n");
    printf("vec1 : ");
    print_vector_int(vec1);

    printf("popping the last value from vec1 ...\n");
    int popped = Vector_pop(&vec1);
    printf("popped value: %d\n", popped);
    printf("vec1 : ");
    print_vector_int(vec1);

    printf("removing the value at index 0 from vec1 ...\n");
    int removed = Vector_remove_at(&vec1, 0);
    printf("value removed: %d\n", removed);
    printf("vec1 : ");
    print_vector_int(vec1);

    printf("removing the first 5 from vec1 ...\n");
    #if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
        index = Vector_remove_value(&vec1, 5, int_boolean_comparator);
    #else
        Vector_remove_value(&vec1, 5, int_boolean_comparator, &index);
    #endif
    printf("index of the removed 5: %d\n", index);
    printf("vec1 : ");
    print_vector_int(vec1);

    printf("copying vec1 to vec2 ...\n");
    #if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
        int *vec2 = Vector_copy(&vec1);
    #else
        int *vec2 = NULL;
        Vector_copy(&vec1, &vec2);
    #endif
    printf("copyed vec1 to vec2\n");
    printf("vec2 : ");
    print_vector_int(vec2);

    printf("sorting vec2 ...\n");
    #if COMPILER_SUPPORTS_TYPEOF
        Vector_sort(&vec2, int_ordering_comparator);
    #else
        Vector_sort(&vec2, int_ordering_comparator, int);
    #endif
    printf("vec2 : ");
    print_vector_int(vec2);


    printf("inserting 5 in its sorted position in vec2 ...\n");
    #if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
        index = Vector_insert_sorted(&vec2, 5, int_ordering_comparator);
    #else
        Vector_insert_sorted(&vec2, 5, int_ordering_comparator, &index);
    #endif
    printf("index of the inserted 5: %d\n", index);
    printf("vec2 : ");
    print_vector_int(vec2);

    printf("reversing vec2 ...\n");
    Vector_reverse(&vec2);
    printf("vec2 : ");
    print_vector_int(vec2);

    printf("filtering vec2 for even numbers and storing the result in vec3 ...\n");
    #if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
        #if COMPILER_SUPPORTS_TYPEOF
            int *vec3 = Vector_filter(&vec2, filter);
        #else
            int *vec3 = Vector_filter(&vec2, filter, int);
        #endif
    #else
        #if COMPILER_SUPPORTS_TYPEOF
            int *vec3 = NULL;
            Vector_filter(&vec2, filter, &vec3);
        #else
            int *vec3 = NULL;
            Vector_filter(&vec2, filter, &vec3, int);
        #endif
    #endif

    printf("vec3 : ");
    print_vector_int(vec3);

    printf("doubling the values in vec3 ...\n");
    Vector_foreach(&vec3, for_each);
    printf("vec3 : ");
    print_vector_int(vec3);

    printf("mapping vec3 by adding 2 to each value and storing the result in vec4 ...\n");
    #if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
        #if COMPILER_SUPPORTS_TYPEOF
            int *vec4 = Vector_map(&vec3, map);
        #else
            int *vec4 = Vector_map(&vec3, map, int);
        #endif
    #else
        #if COMPILER_SUPPORTS_TYPEOF
            int *vec4 = NULL;
            Vector_map(&vec3, map, &vec4);
        #else
            int *vec4 = NULL;
            Vector_map(&vec3, map, &vec4, int);
        #endif
    #endif
    printf("vec4 : ");
    print_vector_int(vec4);


    printf("calculating the sum of vec4 ...\n");
    #if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
        #if COMPILER_SUPPORTS_TYPEOF
            int sum = Vector_reduce(&vec4, sum_reducer, 0);
        #else
            int sum = Vector_reduce(&vec4, sum_reducer, 0, int);
        #endif
    #else
        #if COMPILER_SUPPORTS_TYPEOF
            int sum = 0;
            Vector_reduce(&vec4, sum_reducer, 0, &sum);
        #else
            int sum = 0;
            Vector_reduce(&vec4, sum_reducer, 0, &sum, int);
        #endif
    #endif
    printf("sum of vec4: %d\n", sum);

    printf("checking if any value in vec4 is equal to 10 ...\n");
    #if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
        bool any = Vector_any(&vec4, any_all);
    #else
        bool any = false;
        Vector_any(&vec4, any_all, &any);
    #endif
    printf("any value is equal to 10: %s\n", any ? "true" : "false");

    printf("checking if all values in vec4 are equal to 10 ...\n");
    #if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
        bool all = Vector_all(&vec4, any_all);
    #else
        bool all = false;
        Vector_all(&vec4, any_all, &all);
    #endif
    printf("all values are equal to 10: %s\n", all ? "true" : "false");

    printf("setting all values in vec4 to 10 ...\n");
    Vector_foreach(&vec4, set_all_to_10);
    printf("vec4 : ");
    print_vector_int(vec4);

    printf("checking if all values in vec4 are equal to 10 ...\n");
    #if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
        all = Vector_all(&vec4, any_all);
    #else
        all = false;
        Vector_all(&vec4, any_all, &all);
    #endif
    printf("all values are equal to 10: %s\n", all ? "true" : "false");

    printf("slicing vec4 from index 0 to 10 with a step of 2 and storing the result in vec5 ...\n");
    #if COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS
        #if COMPILER_SUPPORTS_TYPEOF
            int *vec5 = Vector_slice(&vec4, 0, Vector_length(&vec4), 2);
        #else
            int *vec5 = Vector_slice(&vec4, 0, Vector_length(&vec4), 2, int);
        #endif
    #else
        #if COMPILER_SUPPORTS_TYPEOF
            int *vec5 = NULL;
            Vector_slice(&vec4, 0, Vector_length(&vec4), 2, &vec5);
        #else
            int *vec5 = NULL;
            Vector_slice(&vec4, 0, Vector_length(&vec4), 2, &vec5, int);
        #endif
    #endif

    printf("vec5 : ");
    print_vector_int(vec5);

    Vector_destroy(&vec1);
    Vector_destroy(&vec2);
    Vector_destroy(&vec3);
    Vector_destroy(&vec4);
    Vector_destroy(&vec5);
    return 0;
}